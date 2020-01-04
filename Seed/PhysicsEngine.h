#pragma once

class Collider;
class Rigidbody;

class PhysicsEngine
{
public:
    void Raycast() {};

protected:
    friend class Components;

    void Initialize();

    void AddCollider(Collider* collider);
    void AddRigidbody(Rigidbody* rigidbody);

    void RunSimulationStep();
    void OnCollisionUpdate();
    void RigidBodyUpdate();

    void CleanComponents();

private:
    void UpdateSimulationState();
    void Simulate();

    std::vector<Collider*> colliders;
    std::vector<Rigidbody*> rigidbodies;
};
