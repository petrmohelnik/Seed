#pragma once
#include "Component.h"

class btRigidBody;
class Collider;

struct ContactPoint
{
    glm::vec3 Point;
    glm::vec3 Normal;
    float ContactDistance;
    float Impulse;
};

struct Collision
{
    std::vector<ContactPoint> ContactPoints;
    float TotalImpulse;
    Collider* OtherCollider;
};

class Collider : public Component
{
public:

    using Component::Component;
    virtual ~Collider() = default;

    void SetIsTrigger(bool isTrigger);
    bool IsTrigger() const;
    void SetIsKinematic(bool isKinematic);
    bool IsKinematic() const;

    void SetMass(float mass);
    float GetMass() const;

protected:
    friend class PhysicsEngine;

    virtual glm::vec3 GetPosition() const = 0;

    btRigidBody* btRigidbody = nullptr;

    bool isKinematic = false;
    bool isTrigger = false;
    
    float mass = 0.0;

    std::unordered_map<Collider*, std::vector<ContactPoint>> contactPoints;
    std::unordered_map<Collider*, Collision> collisions;
};
