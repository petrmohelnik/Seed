#include "PhysicsEngine.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"
#include "Transform.h"
#include "Engine.h"

btVector3 ToBtVector3(glm::vec3 const& vec3)
{
    return btVector3(vec3.x, vec3.y, vec3.z);
}

btQuaternion ToBtQuaternion(glm::quat const& quat)
{
    return btQuaternion(quat.x, quat.y, quat.z, quat.w);
}

glm::vec3 ToGlmVec3(btVector3 const& vector3)
{
    return glm::vec3(vector3.x(), vector3.y(), vector3.z());
}

glm::quat ToGlmQuat(btQuaternion const& quaternion)
{
    return glm::quat(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
}

void PhysicsEngine::AddCollider(Collider* collider)
{
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

    auto softWorldInfo = dynamicsWorld->getWorldInfo();
    softWorldInfo.air_density = btScalar(1.2f);
    softWorldInfo.water_density = 0;
    softWorldInfo.water_offset = 0;
    softWorldInfo.water_normal = btVector3(0, 0, 0);
    softWorldInfo.m_gravity.setValue(0, -9.81f, 0);
    softWorldInfo.m_sparsesdf.Initialize();
}

btCollisionShape* PhysicsEngine::CreateMeshCollisionShape(MeshCollider* meshCollider)
{
    auto convexHull = new btConvexHullShape();
    for (auto& vertex : meshCollider->GetVertices())
    {
        convexHull->addPoint(ToBtVector3(vertex));
    }
    return convexHull;

    //btTriangleIndexVertexArray* indexVertexArrays = new btTriangleIndexVertexArray(
    //    meshCollider->GetIndices().size(),
    //    &meshCollider->GetIndices()[0].x,
    //    sizeof(glm::uvec3),
    //    meshCollider->GetVertices().size(),
    //    &meshCollider->GetVertices()[0].x,
    //    sizeof(glm::vec3)
    //);

    //bool useQuantizedAabbCompression = true;
    //return std::make_unique<btBvhTriangleMeshShape>(indexVertexArrays, useQuantizedAabbCompression);
}

void PhysicsEngine::UpdateSimulationState()
{
    //inspired by https://github.com/turanszkij/WickedEngine/blob/master/WickedEngine/wiPhysicsEngine_Bullet.cpp
    for (auto collider : colliders)
    {
        if (!collider->btRigidbody)
        {
            btCollisionShape* btShape;
            if (auto boxCollider = dynamic_cast<BoxCollider*>(collider))
                btShape = new btBoxShape(ToBtVector3(boxCollider->GetSize()));
            else if (auto capsuleCollider = dynamic_cast<CapsuleCollider*>(collider))
                btShape = new btCapsuleShape(capsuleCollider->GetRadius(), capsuleCollider->GetHeight());
            else if (auto sphereCollider = dynamic_cast<SphereCollider*>(collider))
                btShape = new btSphereShape(sphereCollider->GetRadius());
            else if (auto meshCollider = dynamic_cast<MeshCollider*>(collider))
                btShape = CreateMeshCollisionShape(meshCollider);
            else
                throw std::runtime_error("Collider type is not supported by PhysicsEngine!");

            btShape->setLocalScaling(ToBtVector3(collider->GetTransform()->GetScale()));

            btTransform btTransform;
            btTransform.setIdentity();
            btTransform.setOrigin(ToBtVector3(collider->GetPosition()));
            btTransform.setRotation(ToBtQuaternion(collider->GetTransform()->GetRotation()));

            auto mass = collider->GetMass();
            bool isDynamic = mass != 0.f && !collider->IsKinematic();

            btVector3 localInertia(0, 0, 0);
            if (isDynamic)
                btShape->calculateLocalInertia(mass, localInertia);
            else
                mass = 0;

            ///using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
            auto btMotionState = new btDefaultMotionState(btTransform);

            btRigidBody::btRigidBodyConstructionInfo btRigidbodyInfo(mass, btMotionState, btShape, localInertia);
            btRigidbodyInfo.m_friction = 0.5;
            btRigidbodyInfo.m_restitution = 1.0; //bounciness

            collider->btRigidbody = new btRigidBody(btRigidbodyInfo);
            collider->btRigidbody->setUserPointer(reinterpret_cast<void*>(collider));

            collider->btRigidbody->setCollisionFlags(collider->btRigidbody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
            if (collider->IsKinematic())
            {
                collider->btRigidbody->setCollisionFlags(collider->btRigidbody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
                collider->btRigidbody->setActivationState(DISABLE_DEACTIVATION);
            }
            if(collider->IsTrigger())
                collider->btRigidbody->setCollisionFlags(collider->btRigidbody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

            dynamicsWorld->addRigidBody(collider->btRigidbody);
        }

        int activationState = collider->btRigidbody->getActivationState();
        if (collider->IsKinematic())
            activationState |= DISABLE_DEACTIVATION;
        else
            activationState &= ~DISABLE_DEACTIVATION;
        collider->btRigidbody->setActivationState(activationState);

        // For kinematic object, system updates physics state, else the physics updates system state
        if (collider->IsKinematic())
        {
            auto btMotionState = collider->btRigidbody->getMotionState();
            btTransform btTransform;
            btTransform.setOrigin(ToBtVector3(collider->GetPosition()));
            btTransform.setRotation(ToBtQuaternion(collider->GetTransform()->GetRotation()));
            btMotionState->setWorldTransform(btTransform);
        }
    }
}

void PhysicsEngine::Simulate()
{
    dynamicsWorld->stepSimulation(Engine::GetTime().FixedDeltaTime(), 1);
}

void PhysicsEngine::OnCollisionUpdate()
{
    for (auto& collider : colliders)
    {
        collider->contactPoints.clear();
    }

    for (int i = 0; i < dynamicsWorld->getDispatcher()->getNumManifolds(); ++i)
    {
        auto contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        auto objectA = reinterpret_cast<Collider*>(contactManifold->getBody0()->getUserPointer());
        auto objectB = reinterpret_cast<Collider*>(contactManifold->getBody1()->getUserPointer());

        for (int j = 0; j < contactManifold->getNumContacts(); ++j)
        {
            auto btContactPoint = contactManifold->getContactPoint(j);

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

            if (objectA->contactPoints.count(objectB) == 0)
                objectA->contactPoints.insert({ objectB , std::vector<ContactPoint>() });
            objectA->contactPoints[objectB].push_back(std::move(contactPointA));

            if (objectB->contactPoints.count(objectA) == 0)
                objectB->contactPoints.insert({ objectA , std::vector<ContactPoint>() });
            objectB->contactPoints[objectA].push_back(std::move(contactPointB));
        }
    }

    for (auto collider : colliders)
    {
        for (auto collision = std::begin(collider->collisions); collision != std::end(collider->collisions);)
        {
            if (collider->contactPoints.count(collision->first) == 0)
            {
                collision->second.ContactPoints.clear();
                for(auto const& script : collider->GetObject()->scripts)
                    script->OnCollisionExit(collision->second);
                collision = collider->collisions.erase(collision);
            }
            else
                ++collision;
        }

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
}

void PhysicsEngine::RigidbodyUpdate()
{
    for (int i = 0; i < dynamicsWorld->getCollisionObjectArray().size(); ++i)
    {
        auto btCollisionObject = dynamicsWorld->getCollisionObjectArray()[i];
        auto collider = reinterpret_cast<Collider*>(btCollisionObject->getUserPointer());

        auto btRigidbody = btRigidBody::upcast(btCollisionObject);
        if (btRigidbody)
        {
            // Feedback non-kinematic objects to system
            if (!collider->IsKinematic())
            {
                auto btMotionState = btRigidbody->getMotionState();
                btTransform btTransform;
                btMotionState->getWorldTransform(btTransform);
                btVector3 btPosition = btTransform.getOrigin();
                btQuaternion btRotation = btTransform.getRotation();

                auto transform = collider->GetTransform();
                transform->SetPosition(ToGlmVec3(btPosition), Transform::Space::World);
                transform->SetRotation(ToGlmQuat(btRotation), Transform::Space::World);
            }
        }
    }
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
