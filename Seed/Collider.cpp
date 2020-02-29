#include "Collider.h"
#include "Transform.h"
#include "Engine.h"

Collider::Collider(const Collider& collider)
    : Component(collider)
    , isKinematic(collider.isKinematic)
    , isTrigger(collider.isTrigger)
    , dirty(collider.dirty)
    , margin(collider.margin)
    , gravity(collider.gravity)
    , mass(collider.mass)
    , bounciness(collider.bounciness)
    , friction(collider.friction)
    , linearDamping(collider.linearDamping)
    , angularDamping(collider.angularDamping)
    , ccdDistanceThreshold(collider.ccdDistanceThreshold)
    , ccdBoundingSphereRadius(collider.ccdBoundingSphereRadius)
{
}

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

void Collider::SetMargin(float margin_)
{
    margin = margin_;
    dirty = true;
}

float Collider::GetMargin()
{
    return margin;
}

bool Collider::IsSleeping()
{
    return btRigidbody && btRigidbody->isActive();
}

void Collider::InitializeRigidbody()
{
    if (!btRigidbody)
        createRigidbody(this);
}

bool Collider::IsRigidbodyInitialized()
{
    return btRigidbody;
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

void Collider::SetContinuousCollisionDetection(float distanceThreshold, float boundingSphereRadius)
{
    ccdDistanceThreshold = distanceThreshold;
    ccdBoundingSphereRadius = boundingSphereRadius;
    if (btRigidbody)
    {
        btRigidbody->setCcdMotionThreshold(ccdDistanceThreshold);
        btRigidbody->setCcdSweptSphereRadius(ccdBoundingSphereRadius);
    }
    dirty = true;
}

float Collider::GetCcdDistanceThreshold()
{
    return ccdDistanceThreshold;
}

float Collider::GetCcdBoundingSphereRadius()
{
    return ccdBoundingSphereRadius;
}

void Collider::AddForce(glm::vec3 force, ForceType forceType)
{
    if (!btRigidbody)
        throw std::runtime_error("Cannot apply force, rigidbody is not initialized");

    btRigidbody->activate();

    if (forceType == ForceType::Force)
        btRigidbody->applyCentralForce(PhysicsEngine::ToBtVector3(force));
    else if (forceType == ForceType::Impulse)
        btRigidbody->applyCentralImpulse(PhysicsEngine::ToBtVector3(force));
    else if (forceType == ForceType::Acceleration)
        btRigidbody->applyCentralForce(PhysicsEngine::ToBtVector3(force) * btRigidbody->getMass());
    else if (forceType == ForceType::VelocityChange)
        btRigidbody->applyCentralImpulse(PhysicsEngine::ToBtVector3(force) * btRigidbody->getMass());
}

void Collider::AddTorque(glm::vec3 torque, ForceType forceType)
{
    if (!btRigidbody)
        throw std::runtime_error("Cannot apply torque, rigidbody is not initialized");

    btRigidbody->activate();

    if (forceType == ForceType::Force)
        btRigidbody->applyTorque(PhysicsEngine::ToBtVector3(torque));
    else if (forceType == ForceType::Impulse)
        btRigidbody->applyTorqueImpulse(PhysicsEngine::ToBtVector3(torque));
    else if (forceType == ForceType::Acceleration)
        btRigidbody->applyTorque(btRigidbody->getInvInertiaTensorWorld().inverse() * PhysicsEngine::ToBtVector3(torque));
    else if (forceType == ForceType::VelocityChange)
        btRigidbody->applyTorqueImpulse(btRigidbody->getInvInertiaTensorWorld().inverse() * PhysicsEngine::ToBtVector3(torque));
}

void Collider::AddForceAtPosition(glm::vec3 force, ForceType forceType, glm::vec3 position, Transform::Space space)
{
    if (!btRigidbody)
        throw std::runtime_error("Cannot apply force, rigidbody is not initialized");

    btRigidbody->activate();

    glm::vec3 worldPosition;
    if (space == Transform::Space::Local)
        worldPosition = GetTransform()->GetLocalToWorldMatrix() * glm::vec4(GetTransform()->GetLocalPosition() + position, 1.0f);
    else
        worldPosition = position;
    auto relativePosition = PhysicsEngine::ToBtVector3(worldPosition) - btRigidbody->getCenterOfMassPosition();

    if (forceType == ForceType::Force)
        btRigidbody->applyForce(PhysicsEngine::ToBtVector3(force), relativePosition);
    else if (forceType == ForceType::Impulse)
        btRigidbody->applyImpulse(PhysicsEngine::ToBtVector3(force), relativePosition);
    else if (forceType == ForceType::Acceleration || forceType == ForceType::VelocityChange)
    {
        AddForce(force, forceType);
        AddTorque(glm::cross(PhysicsEngine::ToGlmVec3(relativePosition), force * PhysicsEngine::ToGlmVec3(btRigidbody->getLinearFactor())), forceType);
    }
}

void Collider::SetVelocity(glm::vec3 velocity)
{
    if (btRigidbody)
    {
        btRigidbody->activate();
        btRigidbody->setLinearVelocity(PhysicsEngine::ToBtVector3(velocity));
    }
}

glm::vec3 Collider::GetVelocity()
{
    if (btRigidbody)
        return PhysicsEngine::ToGlmVec3(btRigidbody->getLinearVelocity());
    else
        return glm::vec3(0.0f);
}

glm::vec3 Collider::GetVelocityInPoint(glm::vec3 point, Transform::Space space)
{
    if (!btRigidbody)
        return glm::vec3();
     
    glm::vec3 worldPosition;
    if (space == Transform::Space::Local)
        worldPosition = GetTransform()->GetLocalToWorldMatrix() * glm::vec4(GetTransform()->GetLocalPosition() + point, 1.0f);
    else
        worldPosition = point;
    auto relativePosition = PhysicsEngine::ToBtVector3(worldPosition) - btRigidbody->getCenterOfMassPosition();

    return PhysicsEngine::ToGlmVec3(btRigidbody->getVelocityInLocalPoint(relativePosition));
}

void Collider::OnInputGraphUpdate()
{
    Engine::GetInput().Text("Collider:");
    Engine::GetInput().InputFloat("Mass", mass, [this] { SetMass(mass); });
    Engine::GetInput().InputFloat("Bounciness", bounciness, [this] { SetBounciness(bounciness); });
    Engine::GetInput().InputFloat("Friction", friction, [this] { SetFriction(friction); });
    Engine::GetInput().InputFloat("LinearDamping", linearDamping, [this] { SetLinearDamping(linearDamping); });
    Engine::GetInput().InputFloat("AngularDamping", angularDamping, [this] { SetAngularDamping(angularDamping); });
}
