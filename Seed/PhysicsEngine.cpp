#include "PhysicsEngine.h"
#include "Collider.h"
#include "Rigidbody.h"

void PhysicsEngine::AddCollider(Collider* collider)
{
    colliders.push_back(collider);
}

void PhysicsEngine::AddRigidbody(Rigidbody* rigidbody)
{
    rigidbodies.push_back(rigidbody);
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
