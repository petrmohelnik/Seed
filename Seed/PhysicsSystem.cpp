#include "PhysicsSystem.h"
#include "Collider.h"
#include "Rigidbody.h"

void PhysicsSystem::AddCollider(std::weak_ptr<Collider> collider)
{
    colliders.push_back(collider);
}

void PhysicsSystem::AddRigidbody(std::weak_ptr<Rigidbody> rigidbody)
{
    rigidbodies.push_back(rigidbody);
}
