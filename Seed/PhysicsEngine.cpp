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
