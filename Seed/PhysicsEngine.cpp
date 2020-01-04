#include "PhysicsEngine.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

void PhysicsEngine::Initialize()
{
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

void PhysicsEngine::RigidBodyUpdate()
{
}

void PhysicsEngine::CleanComponents()
{
    std::experimental::erase_if(colliders, [](const auto collider)
    {
        return collider->ToBeDestroyed();
    });
    std::experimental::erase_if(rigidbodies, [](const auto rigidbody)
    {
        return rigidbody->ToBeDestroyed();
    });
}

void PhysicsEngine::UpdateSimulationState()
{
}

void PhysicsEngine::Simulate()
{
}
