#include "PhysicsEngine.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"
#include "PhysicsObject.h"
#include "Transform.h"
#include "Engine.h"

btVector3 PhysicsEngine::ToBtVector3(glm::vec3 const& vec3)
{
    return btVector3(vec3.x, vec3.y, vec3.z);
}

btQuaternion PhysicsEngine::ToBtQuaternion(glm::quat const& quat)
{
    return btQuaternion(quat.x, quat.y, quat.z, quat.w);
}

glm::vec3 PhysicsEngine::ToGlmVec3(btVector3 const& vector3)
{
    return glm::vec3(vector3.x(), vector3.y(), vector3.z());
}

glm::quat PhysicsEngine::ToGlmQuat(btQuaternion const& quaternion)
{
    return glm::quat(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
}

void PhysicsEngine::AddCollider(Collider* collider)
{
    collider->createRigidbody = [this](Collider* collider) { CreateRigidbody(collider); };
    colliders.push_back(collider);
}

void PhysicsEngine::RunSimulationStep()
{
    UpdateSimulationState();
    Simulate();
}

void PhysicsEngine::Initialize()
{
    ///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
    overlappingPairCache = std::make_unique<btDbvtBroadphase>();
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    collisionConfiguration = std::make_unique<btSoftBodyRigidBodyCollisionConfiguration>();
    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    solver = std::make_unique<btSequentialImpulseConstraintSolver>();

    dynamicsWorld = std::make_unique<btSoftRigidDynamicsWorld>(dispatcher.get(), overlappingPairCache.get(), solver.get(), collisionConfiguration.get());

    dynamicsWorld->getSolverInfo().m_solverMode |= SOLVER_RANDMIZE_ORDER;
    dynamicsWorld->getSolverInfo().m_splitImpulse = true;
    dynamicsWorld->getDispatchInfo().m_enableSatConvex = true;
    dynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
    dynamicsWorld->getPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

    auto softWorldInfo = dynamicsWorld->getWorldInfo();
    softWorldInfo.air_density = btScalar(1.2f);
    softWorldInfo.water_density = 0;
    softWorldInfo.water_offset = 0;
    softWorldInfo.water_normal = btVector3(0, 0, 0);
    softWorldInfo.m_gravity.setValue(0, -9.81f, 0);
    softWorldInfo.m_sparsesdf.Initialize();
}

btCollisionShape* PhysicsEngine::CreateConvexHullShape(MeshCollider * meshCollider)
{
    auto convexHull = new btConvexHullShape();

    for (auto& vertex : meshCollider->GetVertices())
        convexHull->addPoint(ToBtVector3(vertex));

    return convexHull;
}

btCollisionShape* PhysicsEngine::CreateTriangleMeshShape(MeshCollider * meshCollider)
{
    auto triangleMesh = new btTriangleMesh();

    int* btIndices = new int[meshCollider->GetIndices().size() * 3];
    int i = 0;
    for (auto const& triangle : meshCollider->GetIndices())
    {
        btIndices[i++] = triangle.x;
        btIndices[i++] = triangle.y;
        btIndices[i++] = triangle.z;
    }

    btVector3* btVertices = new btVector3[meshCollider->GetVertices().size()];
    i = 0;
    for (auto const& vertex : meshCollider->GetVertices())
    {
        btVertices[i++] = ToBtVector3(vertex);
    }

    btTriangleIndexVertexArray* indexVertexArrays = new btTriangleIndexVertexArray(
        static_cast<int>(meshCollider->GetIndices().size()),
        &btIndices[0],
        3 * sizeof(int),
        static_cast<int>(meshCollider->GetVertices().size()),
        const_cast<btScalar*>(&btVertices[0].x()),
        sizeof(btVector3)
    );

    return new btBvhTriangleMeshShape(indexVertexArrays, true);
}

btCollisionShape* PhysicsEngine::CreateMeshCollisionShape(MeshCollider* meshCollider)
{
    if (meshCollider->IsConvex())
        return CreateConvexHullShape(meshCollider);
    else
        return CreateTriangleMeshShape(meshCollider);
}

btCollisionShape* PhysicsEngine::CreateCollisionShape(Collider* collider)
{
    btCollisionShape* btShape;
    if (auto boxCollider = dynamic_cast<BoxCollider*>(collider))
        btShape = new btBoxShape(ToBtVector3(boxCollider->GetSize()));
    else if (auto capsuleCollider = dynamic_cast<CapsuleCollider*>(collider))
        btShape = new btCapsuleShape(capsuleCollider->GetRadius(), capsuleCollider->GetHeight() - capsuleCollider->GetRadius() * 2.0f);
    else if (auto sphereCollider = dynamic_cast<SphereCollider*>(collider))
        btShape = new btSphereShape(sphereCollider->GetRadius());
    else if (auto meshCollider = dynamic_cast<MeshCollider*>(collider))
        btShape = CreateMeshCollisionShape(meshCollider);
    else
        throw std::runtime_error("Collider type is not supported by PhysicsEngine!");

    return btShape;
}

btVector3 PhysicsEngine::CalculateLocalInertia(Collider* collider, btCollisionShape* btShape)
{
    btVector3 localInertia(0.0f, 0.0f, 0.0f);
    if (collider->IsDynamic())
        btShape->calculateLocalInertia(collider->GetMass(), localInertia);
    return localInertia;
}

btVector3 PhysicsEngine::GetLocalScaling(Collider* collider)
{
    return ToBtVector3(collider->GetTransform()->GetScale());
}

btTransform PhysicsEngine::GetBtTransform(Collider* collider)
{
    return btTransform(ToBtQuaternion(collider->GetTransform()->GetRotation()), ToBtVector3(collider->GetPosition()));
}

void PhysicsEngine::InitializeRigidbodyMaterial(Collider* collider)
{
    collider->btRigidbody->setRestitution(collider->GetBounciness());
    collider->btRigidbody->setFriction(collider->GetFriction());
    collider->btRigidbody->setGravity(ToBtVector3(collider->GetGravity()));
    collider->btRigidbody->setDamping(collider->GetLinearDamping(), collider->GetAngularDamping());
    collider->btRigidbody->setCcdMotionThreshold(collider->GetCcdDistanceThreshold());
    collider->btRigidbody->setCcdSweptSphereRadius(collider->GetCcdBoundingSphereRadius());
}

void PhysicsEngine::CreateRigidbody(Collider* collider)
{
    auto btShape = CreateCollisionShape(collider);
    btShape->setLocalScaling(GetLocalScaling(collider));
    auto localInertia = CalculateLocalInertia(collider, btShape);

    auto btMotionState = new btDefaultMotionState(GetBtTransform(collider));
    collider->btRigidbody = new btRigidBody(btRigidBody::btRigidBodyConstructionInfo(collider->GetMass(), btMotionState, btShape, localInertia));
    collider->btRigidbody->setUserPointer(reinterpret_cast<void*>(collider));

    if (collider->IsKinematic())
    {
        collider->btRigidbody->setCollisionFlags(collider->btRigidbody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
        collider->btRigidbody->setActivationState(DISABLE_DEACTIVATION);
    }
    if (collider->IsTrigger())
        collider->btRigidbody->setCollisionFlags(collider->btRigidbody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    InitializeRigidbodyMaterial(collider);

    dynamicsWorld->addRigidBody(collider->btRigidbody);
    collider->btDynamicsWorld = dynamicsWorld.get();

    if (auto physicsObject = dynamic_cast<PhysicsObject*>(collider))
        physicsObject->InitializePhysics();
    else
        collider->dirty = false;
}

void PhysicsEngine::RefreshRigidbody(Collider* collider)
{
    dynamicsWorld->removeRigidBody(collider->btRigidbody);

    collider->btRigidbody->getCollisionShape()->setMargin(collider->GetMargin());
    collider->btRigidbody->getCollisionShape()->setLocalScaling(GetLocalScaling(collider));
    btVector3 localInertia = CalculateLocalInertia(collider, collider->btRigidbody->getCollisionShape());
    collider->btRigidbody->setMassProps(collider->GetMass(), localInertia);

    dynamicsWorld->addRigidBody(collider->btRigidbody);
}

void PhysicsEngine::UpdateSimulationState()
{
    for (auto collider : colliders)
    {
        if (!collider->btRigidbody)
        {
            CreateRigidbody(collider);
        }

        if (collider->dirty)
        {
            RefreshRigidbody(collider);
            collider->dirty = false;
        }

        if (collider->IsKinematic())
        {
            collider->btRigidbody->getMotionState()->setWorldTransform(GetBtTransform(collider));
        }

        if (auto physicsObject = dynamic_cast<PhysicsObject*>(collider))
        {
            physicsObject->BeforeSimulationUpdate();
        }
    }
}

void PhysicsEngine::Simulate()
{
    dynamicsWorld->stepSimulation(Engine::GetTime().FixedDeltaTime(), 0);
}

void PhysicsEngine::InsertContactPoint(Collider* thisCollider, Collider* otherCollider, ContactPoint contactPoint)
{
    if (thisCollider->contactPoints.count(otherCollider) == 0)
        thisCollider->contactPoints.insert({ otherCollider , std::vector<ContactPoint>() });
    thisCollider->contactPoints[otherCollider].push_back(std::move(contactPoint));
}

void PhysicsEngine::AddContactManifold(btPersistentManifold* contactManifold)
{
    auto colliderA = reinterpret_cast<Collider*>(contactManifold->getBody0()->getUserPointer());
    auto colliderB = reinterpret_cast<Collider*>(contactManifold->getBody1()->getUserPointer());
    if (!colliderA || !colliderB)
        return;

    for (int contactNumber = 0; contactNumber < contactManifold->getNumContacts(); ++contactNumber)
    {
        auto btContactPoint = contactManifold->getContactPoint(contactNumber);

        ContactPoint contactPointA;
        contactPointA.Point = ToGlmVec3(btContactPoint.getPositionWorldOnA());
        contactPointA.Normal = -ToGlmVec3(btContactPoint.m_normalWorldOnB);
        contactPointA.ContactDistance = btContactPoint.getDistance();
        contactPointA.Impulse = btContactPoint.getAppliedImpulse();

        ContactPoint contactPointB;
        contactPointB.Point = ToGlmVec3(btContactPoint.getPositionWorldOnB());
        contactPointB.Normal = ToGlmVec3(btContactPoint.m_normalWorldOnB);
        contactPointB.ContactDistance = btContactPoint.getDistance();
        contactPointB.Impulse = btContactPoint.getAppliedImpulse();

        InsertContactPoint(colliderA, colliderB, std::move(contactPointA));
        InsertContactPoint(colliderB, colliderA, std::move(contactPointB));
    }
}

void PhysicsEngine::ProcessCollisionExit(Collider* collider)
{
    for (auto collision = std::begin(collider->collisions); collision != std::end(collider->collisions);)
    {
        if (collider->contactPoints.count(collision->first) == 0)
        {
            collision->second.ContactPoints.clear();
            for (auto const& script : collider->GetObject()->scripts)
                script->OnCollisionExit(collision->second);

            collision = collider->collisions.erase(collision);
        }
        else
            ++collision;
    }
}

void PhysicsEngine::ProcessCollisionEnterAndStay(Collider* collider)
{
    for (auto& contactPoints : collider->contactPoints)
    {
        if (collider->collisions.count(contactPoints.first) == 0)
        {
            auto& collision = collider->collisions.insert({ contactPoints.first, Collision{} }).first->second;

            collision.ContactPoints = std::move(contactPoints.second);
            for (auto const& contactPoint : collision.ContactPoints)
                collision.TotalImpulse += contactPoint.Impulse;
            collision.OtherCollider = contactPoints.first;

            for (auto const& script : collider->GetObject()->scripts)
                script->OnCollisionEnter(collision);
        }
        else
        {
            auto& collision = collider->collisions[contactPoints.first];

            collision.ContactPoints = std::move(contactPoints.second);
            for (auto const& contactPoint : collision.ContactPoints)
                collision.TotalImpulse += contactPoint.Impulse;

            for (auto const& script : collider->GetObject()->scripts)
                script->OnCollisionStay(collision);
        }
    }
}

void PhysicsEngine::OnCollisionUpdate()
{
    for (auto& collider : colliders)
    {
        collider->contactPoints.clear();
        if (auto physicsObject = dynamic_cast<PhysicsObject*>(collider))
            physicsObject->AfterSimulationUpdate();
    }

    for (int manifoldNumber = 0; manifoldNumber < dynamicsWorld->getDispatcher()->getNumManifolds(); ++manifoldNumber)
    {
        AddContactManifold(dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(manifoldNumber));
    }

    for (auto collider : colliders)
    {
        ProcessCollisionExit(collider);
        ProcessCollisionEnterAndStay(collider);
    }
}

void PhysicsEngine::RigidbodyUpdate()
{
    for (int i = 0; i < dynamicsWorld->getCollisionObjectArray().size(); ++i)
    {
        if (auto btRigidbody = btRigidBody::upcast(dynamicsWorld->getCollisionObjectArray()[i]))
        {
            auto collider = reinterpret_cast<Collider*>(btRigidbody->getUserPointer());

            if (!collider->IsKinematic())
            {
                btTransform btTransform;
                btRigidbody->getMotionState()->getWorldTransform(btTransform);

                collider->SetPosition(ToGlmVec3(btTransform.getOrigin()));
                collider->GetTransform()->SetRotation(ToGlmQuat(btTransform.getRotation()), Transform::Space::World);
            }
        }
    }
}

void PhysicsEngine::OnMouseUpdate()
{
}

void PhysicsEngine::CleanComponents()
{
    std::experimental::erase_if(colliders, [this](const auto collider)
    {
        if (collider->ToBeDestroyed() && collider->btRigidbody)
        {
            if (collider->btRigidbody->getCollisionShape())
                delete collider->btRigidbody->getCollisionShape();
            if (collider->btRigidbody->getMotionState())
                delete collider->btRigidbody->getMotionState();
            dynamicsWorld->removeCollisionObject(collider->btRigidbody);

            delete collider->btRigidbody;
        }
        return collider->ToBeDestroyed();
    });
}
