#include "Collider.h"
#include "Object.h"
#include "Transform.h"

void Collider::SetIsKinematic(bool isKinematic_)
{
    isKinematic = isKinematic_;
    if (btRigidbody)
    {
        auto collisionFlags = btRigidbody->getCollisionFlags();
        auto activationState = btRigidbody->getActivationState();

        if (isKinematic)
        {
            collisionFlags |= btCollisionObject::CF_KINEMATIC_OBJECT;
            activationState |= DISABLE_DEACTIVATION;
        }
        else
        {
            collisionFlags &= ~btCollisionObject::CF_KINEMATIC_OBJECT;
            activationState &= ~DISABLE_DEACTIVATION;
        }

        btRigidbody->setCollisionFlags(collisionFlags);
        btRigidbody->setActivationState(activationState);
    }
    dirty = true;
}

bool Collider::IsKinematic() const
{
    return isKinematic;
}

bool Collider::IsDynamic() const
{
    return mass != 0.0 && !IsKinematic();
}

bool Collider::IsStatic() const
{
    return mass == 0.0 && !IsKinematic();;
}

void Collider::SetIsTrigger(bool isTrigger_)
{
    isTrigger = isTrigger_;
    if (btRigidbody)
    {
        auto collisionFlags = btRigidbody->getCollisionFlags();

        if (isTrigger)
            collisionFlags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
        else
            collisionFlags &= ~btCollisionObject::CF_NO_CONTACT_RESPONSE;

        btRigidbody->setCollisionFlags(collisionFlags);
    }
}

bool Collider::IsTrigger() const
{
    return isTrigger;
}

void Collider::SetMass(float mass_)
{
    mass = mass_;
    dirty = true;
}

float Collider::GetMass() const
{
    return mass;
}

void Collider::UseGravity(bool useGravity)
{
    gravity = useGravity ? glm::vec3(0.0f, -9.81f, 0.0f) : glm::vec3(0.0f);
    if (btRigidbody)
        btRigidbody->setGravity(PhysicsEngine::ToBtVector3(gravity));
}

glm::vec3 Collider::GetGravity()
{
    return gravity;
}

void Collider::SetBounciness(float bounciness_)
{
    bounciness = bounciness_;
    if (btRigidbody)
        btRigidbody->setRestitution(bounciness);
}

float Collider::GetBounciness()
{
    return bounciness;
}

void Collider::SetFriction(float friction_)
{
    friction = friction_;
    if (btRigidbody)
        btRigidbody->setFriction(friction);
}

float Collider::GetFriction()
{
    return friction;
}

void Collider::SetLinearDamping(float linearDamping_)
{
    linearDamping = linearDamping_;
    if (btRigidbody)
        btRigidbody->setDamping(linearDamping, angularDamping);
}

float Collider::GetLinearDamping()
{
    return linearDamping;
}

void Collider::SetAngularDamping(float angularDamping_)
{
    angularDamping = angularDamping_;
    if (btRigidbody)
        btRigidbody->setDamping(linearDamping, angularDamping);
}

float Collider::GetAngularDamping()
{
    return angularDamping;
}
