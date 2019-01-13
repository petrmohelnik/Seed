#pragma once

class Collider;
class Rigidbody;

class PhysicsEngine
{
public:
    void Raycast() {};

protected:
    friend class Components;

    void AddCollider(std::weak_ptr<Collider> collider);
    void AddRigidbody(std::weak_ptr<Rigidbody> rigidbody);

    void CleanComponents() {};

private:
    std::vector<std::weak_ptr<Collider>> colliders;
    std::vector<std::weak_ptr<Rigidbody>> rigidbodies;
};
