#pragma once
#include "Component.h"
#include "Transform.h"

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

    enum class ForceType
    {
        Force,
        Impulse,
        Acceleration,
        VelocityChange
    };

    void SetIsTrigger(bool isTrigger);
    bool IsTrigger() const;
    void SetIsKinematic(bool isKinematic);
    bool IsKinematic() const;
    bool IsDynamic() const;
    bool IsStatic() const;
    void SetMargin(float margin);
    float GetMargin();
    bool IsSleeping();
    void InitializeRigidbody();

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
    void SetContinuousCollisionDetection(float distanceThreshold, float boundingSphereRadius);
    float GetCcdDistanceThreshold();
    float GetCcdBoundingSphereRadius();

    void AddForce(glm::vec3 force, ForceType forceType);
    void AddTorque(glm::vec3 torque, ForceType forceType);
    void AddForceAtPosition(glm::vec3 force, ForceType forceType, glm::vec3 position, Transform::Space space = Transform::Space::World);
    void SetVelocity(glm::vec3 velocity);
    glm::vec3 GetVelocity();
    glm::vec3 GetVelocityInPoint(glm::vec3 point, Transform::Space space = Transform::Space::World);

protected:
    friend class PhysicsEngine;
    friend class Transform;

    virtual glm::vec3 GetPosition() const = 0;
    virtual void SetPosition(glm::vec3 position) = 0;
    void OnInputGraphUpdate() override;

    btRigidBody* btRigidbody = nullptr;
    btSoftRigidDynamicsWorld* btDynamicsWorld = nullptr;
    std::function<void(Collider*)> createRigidbody;

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
    float ccdDistanceThreshold = 0.0f;
    float ccdBoundingSphereRadius = 0.0f;

    std::unordered_map<Collider*, std::vector<ContactPoint>> contactPoints;
    std::unordered_map<Collider*, Collision> collisions;
};
