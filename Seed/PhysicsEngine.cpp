#include "PhysicsEngine.h"
#include "Collider.h"
#include "Rigidbody.h"

void PhysicsEngine::AddCollider(std::weak_ptr<Collider> collider)
{
    colliders.push_back(collider);
}

void PhysicsEngine::AddRigidbody(std::weak_ptr<Rigidbody> rigidbody)
{
    rigidbodies.push_back(rigidbody);
}
