#pragma once

class Collider;
class Rigidbody;

class PhysicsSystem
{
public:
    void AddCollider(std::weak_ptr<Collider> collider);
    void AddRigidbody(std::weak_ptr<Rigidbody> rigidbody);

private:
    std::vector<std::weak_ptr<Collider>> colliders;
    std::vector<std::weak_ptr<Rigidbody>> rigidbodies;
};
