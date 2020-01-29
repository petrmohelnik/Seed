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

bool PhysicsEngine::Raycast(glm::vec3 fromPosition, glm::vec3 direction, RaycastHit& hitInfo, float maxDistance)
{
    auto btFromPosition = ToBtVector3(fromPosition);
    auto btToPosition = ToBtVector3(glm::normalize(direction) * maxDistance);

    btCollisionWorld::ClosestRayResultCallback closestResultCallback(btFromPosition, btToPosition);
    dynamicsWorld->rayTest(btFromPosition, btToPosition, closestResultCallback);
    if (closestResultCallback.hasHit())
    {
        hitInfo.Collider = reinterpret_cast<Collider*>(closestResultCallback.m_collisionObject->getUserPointer());
        hitInfo.Point = ToGlmVec3(closestResultCallback.m_hitPointWorld);
        hitInfo.Normal = ToGlmVec3(closestResultCallback.m_hitNormalWorld);
        return true;
    }

    return false;
}

bool PhysicsEngine::Raycast(glm::vec3 fromPosition, glm::vec3 direction, float maxDistance)
{
    RaycastHit dummyInfo;
    return Raycast(fromPosition, direction, dummyInfo, maxDistance);
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
    
    btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher.get());

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

btTriangleIndexVertexArray* PhysicsEngine::CreateTriangleArray(MeshCollider* meshCollider, int submeshIndex)
{
    auto btIndices = new int[meshCollider->GetIndices(submeshIndex).size() * 3];
    int i = 0;
    for (auto const& triangle : meshCollider->GetIndices(submeshIndex))
    {
        btIndices[i++] = triangle.x;
        btIndices[i++] = triangle.y;
        btIndices[i++] = triangle.z;
    }

    auto btVertices = new btVector3[meshCollider->GetVertices(submeshIndex).size()];
    i = 0;
    for (auto const& vertex : meshCollider->GetVertices(submeshIndex))
    {
        btVertices[i++] = ToBtVector3(vertex);
    }

    auto indexVertexArrays = new btTriangleIndexVertexArray(
        static_cast<int>(meshCollider->GetIndices(submeshIndex).size()),
        &btIndices[0],
        3 * sizeof(int),
        static_cast<int>(meshCollider->GetVertices(submeshIndex).size()),
        const_cast<btScalar*>(&btVertices[0].x()),
        sizeof(btVector3)
    );

    return indexVertexArrays;
}

btCollisionShape* PhysicsEngine::CreateGImpactShape(MeshCollider* meshCollider, int submeshIndex)
{
    auto triangleVertexArray = CreateTriangleArray(meshCollider, submeshIndex);
    auto gImpactMeshShape = new btGImpactMeshShape(triangleVertexArray);
    gImpactMeshShape->updateBound();
    return gImpactMeshShape;
}

btCollisionShape* PhysicsEngine::CreateConvexHullShape(MeshCollider* meshCollider, int submeshIndex)
{
    auto convexHullShape = new btConvexHullShape();
    for (auto& vertex : meshCollider->GetVertices(submeshIndex))
        convexHullShape->addPoint(ToBtVector3(vertex));
    return convexHullShape;
}

btCollisionShape* PhysicsEngine::CreateTriangleMeshShape(MeshCollider* meshCollider, int submeshIndex)
{
    auto triangleVertexArray = CreateTriangleArray(meshCollider, submeshIndex);
    return new btBvhTriangleMeshShape(triangleVertexArray, true);
}

btCollisionShape* PhysicsEngine::CreateMeshCollisionSubShape(MeshCollider* meshCollider, int submeshIndex)
{
    if (meshCollider->GetMass() != 0.0f)
    {
        if (meshCollider->IsConvex())
            return CreateConvexHullShape(meshCollider, submeshIndex);
        else
            return CreateGImpactShape(meshCollider, submeshIndex);
    }
    else
        return CreateTriangleMeshShape(meshCollider, submeshIndex);
}

btCollisionShape* PhysicsEngine::CreateMeshCollisionShape(MeshCollider* meshCollider)
{
    if (meshCollider->NumberOfSubmeshes() == 1)
        return CreateMeshCollisionSubShape(meshCollider, 0);

    auto compoundShape = new btCompoundShape();
    for (int submeshIndex = 0; submeshIndex < meshCollider->NumberOfSubmeshes(); submeshIndex++)
    {
        auto subShape = CreateMeshCollisionSubShape(meshCollider, submeshIndex);
        auto identityTransform = btTransform(ToBtQuaternion(glm::angleAxis(0.0f, glm::vec3(0.0, 0.0, 1.0))), ToBtVector3(glm::vec3(0.0f)));
        compoundShape->addChildShape(identityTransform, subShape);
    }
    return compoundShape;
}

btCollisionShape* PhysicsEngine::CreateCollisionShape(Collider* collider)
{
    btCollisionShape* btShape;

    if (collider->GetType() == Collider::Type::Mesh || collider->GetType() == Collider::Type::CompoundMesh)
    {
        auto meshCollider = static_cast<MeshCollider*>(collider);
        if (!meshCollider->mesh->btCollisionShape.expired())
        {
            meshCollider->btShape = meshCollider->mesh->btCollisionShape.lock();
            btShape = meshCollider->btShape.get();
        }
        else
        {
            btShape = CreateMeshCollisionShape(meshCollider);
            meshCollider->AssignBtCollisionShape(btShape);
        }

    }
    else if (collider->GetType() == Collider::Type::Box)
    {
        auto boxCollider = static_cast<BoxCollider*>(collider);
        btShape = new btBoxShape(ToBtVector3(boxCollider->GetSize()));
    }
    else if (collider->GetType() == Collider::Type::Capsule)
    {
        auto capsuleCollider = static_cast<CapsuleCollider*>(collider);
        btShape = new btCapsuleShape(capsuleCollider->GetRadius(), capsuleCollider->GetHeight() - capsuleCollider->GetRadius() * 2.0f);
    }
    else if (collider->GetType() == Collider::Type::Sphere)
    {
        auto sphereCollider = static_cast<SphereCollider*>(collider);
        btShape = new btSphereShape(sphereCollider->GetRadius());
    }
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

    auto collisionShape = collider->btRigidbody->getCollisionShape();
    collisionShape->setMargin(collider->GetMargin());
    collisionShape->setLocalScaling(GetLocalScaling(collider));
    btVector3 localInertia = CalculateLocalInertia(collider, collisionShape);
    collider->btRigidbody->setMassProps(collider->GetMass(), localInertia);
    if(collisionShape->getShapeType() == GIMPACT_SHAPE_PROXYTYPE)
        static_cast<btGImpactCompoundShape*>(collisionShape)->updateBound();

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
            CallScriptCallbackOnCollider(collider, [&collision](auto script) { script->OnCollisionExit(collision->second); });

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

            CallScriptCallbackOnCollider(collider, [&collision](auto script) { script->OnCollisionEnter(collision); });
        }
        else
        {
            auto& collision = collider->collisions[contactPoints.first];

            collision.ContactPoints = std::move(contactPoints.second);
            for (auto const& contactPoint : collision.ContactPoints)
                collision.TotalImpulse += contactPoint.Impulse;

            CallScriptCallbackOnCollider(collider, [&collision](auto script) { script->OnCollisionStay(collision); });
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

Collider* PhysicsEngine::ClosestColliderUnderMouse()
{
    auto mainCamera = RenderingPipeline::MainCamera();
    auto fromPosition = ToBtVector3(mainCamera->ScreenPositionToWorld(glm::vec3(Engine::GetInput().MousePosition(), mainCamera->GetNearPlane())));
    auto toPosition = ToBtVector3(mainCamera->ScreenPositionToWorld(glm::vec3(Engine::GetInput().MousePosition(), mainCamera->GetFarPlane())));

    btCollisionWorld::ClosestRayResultCallback closestResultCallback(fromPosition, toPosition);
    dynamicsWorld->rayTest(fromPosition, toPosition, closestResultCallback);
    if (closestResultCallback.hasHit())
    {
        return reinterpret_cast<Collider*>(closestResultCallback.m_collisionObject->getUserPointer());
    }

    return nullptr;
}

void PhysicsEngine::OnMouseUpdate()
{
    auto currentColliderUnderMouse = ClosestColliderUnderMouse();

    if (currentColliderUnderMouse)
    {
        if (Engine::GetInput().MouseButtonDown(SDL_BUTTON_LEFT))
        {
            CallScriptCallbackOnCollider(currentColliderUnderMouse, [](auto script) { script->OnMouseDown(); });
            mouseDown = true;
        }
        if (Engine::GetInput().MouseButton(SDL_BUTTON_LEFT))
        {
            CallScriptCallbackOnCollider(currentColliderUnderMouse, [](auto script) { script->OnMouseDrag(); });
        }
        else if (mouseDown)
        {
            CallScriptCallbackOnCollider(currentColliderUnderMouse, [](auto script) { script->OnMouseUp(); });
            mouseDown = false;
        }

        if (currentColliderUnderMouse != previousColliderUnderMouse)
        {
            CallScriptCallbackOnCollider(currentColliderUnderMouse, [](auto script) { script->OnMouseEnter(); });
        }

        CallScriptCallbackOnCollider(currentColliderUnderMouse, [](auto script) { script->OnMouseOver(); });
    }
    if (previousColliderUnderMouse && previousColliderUnderMouse != currentColliderUnderMouse)
    {
        CallScriptCallbackOnCollider(previousColliderUnderMouse, [](auto script) { script->OnMouseExit(); });
    }

    previousColliderUnderMouse = currentColliderUnderMouse;
}


void PhysicsEngine::CallScriptCallbackOnCollider(Collider* collider, std::function<void(Script*)> scriptCallback)
{
    for (auto const& script : collider->GetObject()->scripts)
        scriptCallback(script.get());
}

void PhysicsEngine::CleanComponents()
{
    std::experimental::erase_if(colliders, [this](const auto collider)
    {
        if (collider->ToBeDestroyed() && collider->btRigidbody)
        {
            if (collider->GetType() != Collider::Type::Mesh && collider->GetType() != Collider::Type::CompoundMesh)
                delete collider->btRigidbody->getCollisionShape();

            if (collider->btRigidbody->getMotionState())
                delete collider->btRigidbody->getMotionState();
            dynamicsWorld->removeCollisionObject(collider->btRigidbody);

            delete collider->btRigidbody;
        }
        if (collider->ToBeDestroyed() && collider == previousColliderUnderMouse)
            previousColliderUnderMouse = nullptr;
        return collider->ToBeDestroyed();
    });
}
