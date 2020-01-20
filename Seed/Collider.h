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
    bool IsDynamic() const;
    bool IsStatic() const;
    void SetMargin(float margin);
    float GetMargin();

    void SetMass(float mass);
    float GetMass() const;
    void UseGravity(bool useGravity);
    glm::vec3 GetGravity();
    void SetBounciness(float bounciness);
    float GetBounciness();
    void SetFriction(float friction);
    float GetFriction();
    void SetLinearDamping(float linearDamping);
    float GetLinearDamping();
    void SetAngularDamping(float angularDamping);
    float GetAngularDamping();

protected:
    friend class PhysicsEngine;
    friend class Transform;

    virtual glm::vec3 GetPosition() const = 0;
    virtual void SetPosition(glm::vec3 position) = 0;
	void OnInputGraphUpdate() override;

    btRigidBody* btRigidbody = nullptr;
    btSoftRigidDynamicsWorld* btDynamicsWorld = nullptr;

    bool isKinematic = false;
    bool isTrigger = false;
    bool dirty = true;
    float margin = 0.04f;
    glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
    
    float mass = 0.0;
    float bounciness = 0.0f;
    float friction = 0.5f;
    float linearDamping = 0.0f;
    float angularDamping = 0.0f;

    std::unordered_map<Collider*, std::vector<ContactPoint>> contactPoints;
    std::unordered_map<Collider*, Collision> collisions;
};
