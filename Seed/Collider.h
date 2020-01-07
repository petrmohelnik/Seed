#pragma once
#include "Component.h"

class Rigidbody;
class btRigidBody;
class btCollisionShape;

class Collider : public Component
{
public:
    using Component::Component;
    virtual ~Collider() = default;

protected:
    friend class PhysicsEngine;

    Rigidbody* GetRigidbody();
    virtual glm::vec3 GetPosition() = 0;

    btRigidBody* btRigidbody = nullptr;
    std::unique_ptr<btCollisionShape> btShape;
};
