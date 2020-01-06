#include "PhysicsEngine.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "CapsuleCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"
#include "Rigidbody.h"
#include "Transform.h"
#include "Engine.h"

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
	dynamicsWorld->setGravity(btVector3(0, -9.81, 0));

    auto softWorldInfo = dynamicsWorld->getWorldInfo();
    softWorldInfo.air_density = btScalar(1.2f);
    softWorldInfo.water_density = 0;
    softWorldInfo.water_offset = 0;
    softWorldInfo.water_normal = btVector3(0, 0, 0);
    softWorldInfo.m_gravity.setValue(0, -9.81, 0);
    softWorldInfo.m_sparsesdf.Initialize();
}

void PhysicsEngine::AddCollider(Collider* collider)
{
    colliders.push_back(collider);
}

void PhysicsEngine::AddRigidbody(Rigidbody* rigidbody)
{
    rigidbodies.push_back(rigidbody);
}

void PhysicsEngine::RunSimulationStep()
{
    UpdateSimulationState();
    Simulate();
}

void PhysicsEngine::OnCollisionUpdate()
{
}

void PhysicsEngine::RigidbodyUpdate()
{
    for (int i = 0; i < dynamicsWorld->getCollisionObjectArray().size(); ++i)
    {
        btCollisionObject* btCollisionObject = dynamicsWorld->getCollisionObjectArray()[i];
        auto collider = reinterpret_cast<Collider*>(btCollisionObject->getUserPointer());

        btRigidBody* btRigidbody = btRigidBody::upcast(btCollisionObject);
        if (btRigidbody != nullptr)
        {
            if (auto rigidbody = collider->GetRigidbody())
            {
                // Feedback non-kinematic objects to system
                if (!rigidbody->GetIsKinematic())
                {
                    btMotionState* btMotionState = btRigidbody->getMotionState();
                    btTransform btTransform;

                    btMotionState->getWorldTransform(btTransform);
                    btVector3 btPosition = btTransform.getOrigin();
                    btQuaternion btRotation = btTransform.getRotation();

                    auto transform = collider->GetTransform();
                    transform->SetPosition(glm::vec3(btPosition.x(), btPosition.y(), btPosition.z()), Transform::Space::World);
                    transform->SetRotation(glm::quat(btRotation.x(), btRotation.y(), btRotation.z(), btRotation.w()));
                }
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
            if (collider->btRigidbody->getMotionState())
                delete collider->btRigidbody->getMotionState();
            dynamicsWorld->removeCollisionObject(collider->btRigidbody);
            
            delete collider->btRigidbody;
            collider->btShape.reset();
        }

        return collider->ToBeDestroyed();
    });
    std::experimental::erase_if(rigidbodies, [](const auto rigidbody)
    {
        return rigidbody->ToBeDestroyed();
    });
}

std::unique_ptr<btCollisionShape> PhysicsEngine::CreateBoxCollisionShape(BoxCollider* boxCollider)
{
    return std::make_unique<btBoxShape>(btVector3(boxCollider->GetSize().x, boxCollider->GetSize().y, boxCollider->GetSize().z));
}

std::unique_ptr<btCollisionShape> PhysicsEngine::CreateCapsuleCollisionShape(CapsuleCollider* capsuleCollider)
{
    return std::make_unique<btCapsuleShape>(capsuleCollider->GetRadius(), capsuleCollider->GetHeight());
}

std::unique_ptr<btCollisionShape> PhysicsEngine::CreateSphereCollisionShape(SphereCollider* sphereCollider)
{
    return std::make_unique<btSphereShape>(sphereCollider->GetRadius());
}

std::unique_ptr<btCollisionShape> PhysicsEngine::CreateMeshCollisionShape(MeshCollider* meshCollider)
{
    auto convexHull = std::make_unique<btConvexHullShape>();
    for (auto& vertex : meshCollider->GetVertices())
    {
        convexHull->addPoint(btVector3(vertex.x, vertex.y, vertex.z));
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
        if (!collider->btRigidbody) //create btRigidBody
        {
            if (auto boxCollider = dynamic_cast<BoxCollider*>(collider))
                collider->btShape = CreateBoxCollisionShape(boxCollider);
            else if (auto capsuleCollider = dynamic_cast<CapsuleCollider*>(collider))
                collider->btShape = CreateCapsuleCollisionShape(capsuleCollider);
            else if (auto sphereCollider = dynamic_cast<SphereCollider*>(collider))
                collider->btShape = CreateSphereCollisionShape(sphereCollider);
            else if (auto meshCollider = dynamic_cast<MeshCollider*>(collider))
                collider->btShape = CreateMeshCollisionShape(meshCollider);

            auto scale = collider->GetTransform()->GetScale();
            collider->btShape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));

            if (auto rigidbody = collider->GetRigidbody())
            {
                auto mass = rigidbody->GetMass();
                bool isDynamic = mass != 0.f && !rigidbody->GetIsKinematic();

                btVector3 localInertia(0, 0, 0);
                if (isDynamic)
                    collider->btShape->calculateLocalInertia(mass, localInertia);
                else
                    mass = 0;

                ///using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
                auto transform = collider->GetTransform();
                btTransform btTransform;
                btTransform.setIdentity();
                btTransform.setOrigin(btVector3(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z));
                btTransform.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().x, transform->GetRotation().z, transform->GetRotation().w));
                btDefaultMotionState* btMotionState = new btDefaultMotionState(btTransform);

                btRigidBody::btRigidBodyConstructionInfo btRigidbodyInfo(mass, btMotionState, collider->btShape.get(), localInertia);
                btRigidbodyInfo.m_friction = 0.5;
                btRigidbodyInfo.m_restitution = 1.0; //bounciness
                //rbInfo.m_linearDamping = physicscomponent.damping;
                //rbInfo.m_angularDamping = physicscomponent.damping;

                btRigidBody* btRigidbody = new btRigidBody(btRigidbodyInfo);
                btRigidbody->setUserPointer(reinterpret_cast<void*>(collider));

                if (rigidbody->GetIsKinematic())
                {
                    btRigidbody->setCollisionFlags(btRigidbody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
                    btRigidbody->setActivationState(DISABLE_DEACTIVATION);
                }

                dynamicsWorld->addRigidBody(btRigidbody);
                collider->btRigidbody = btRigidbody;
            }
            else //only collider btCollisionObject
            {
            }
        }

        if (auto rigidbody = collider->GetRigidbody())
        {
            auto btRigidbody = collider->btRigidbody;

            int activationState = btRigidbody->getActivationState();
            if (rigidbody->GetIsKinematic())
                activationState |= DISABLE_DEACTIVATION;
            else
                activationState &= ~DISABLE_DEACTIVATION;
            btRigidbody->setActivationState(activationState);

            // For kinematic object, system updates physics state, else the physics updates system state
            if (rigidbody->GetIsKinematic())
            {
                auto transform = collider->GetTransform();

                btMotionState* btMotionState = btRigidbody->getMotionState();
                btTransform btTransform;

                btTransform.setOrigin(btVector3(transform->GetPosition().x, transform->GetPosition().y, transform->GetPosition().z));
                btTransform.setRotation(btQuaternion(transform->GetRotation().x, transform->GetRotation().x, transform->GetRotation().z, transform->GetRotation().w));
                btMotionState->setWorldTransform(btTransform);
            }
        }
        else //only collider btCollisionObject
        {
        }
    }
}

void PhysicsEngine::Simulate()
{
    dynamicsWorld->stepSimulation(Engine::GetTime().FixedDeltaTime());
}
