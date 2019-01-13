#pragma once

class Collider;
class Rigidbody;

class PhysicsEngine
{
public:
    void Raycast() {};

protected:
    friend class Components;

    void AddCollider(Collider* collider);
    void AddRigidbody(Rigidbody* rigidbody);

    void CleanComponents();

private:
    std::vector<Collider*> colliders;
    std::vector<Rigidbody*> rigidbodies;
};
