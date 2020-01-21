#include "DynamicCharacterController.h"
#include "PhysicsEngine.h"
#include "Engine.h"

class IgnoreBodyAndGhostCast :
    public btCollisionWorld::ClosestRayResultCallback
{
private:
    btRigidBody* pBody;
    btPairCachingGhostObject* btGhostObject;

public:
    IgnoreBodyAndGhostCast(btRigidBody* pBody, btPairCachingGhostObject* btGhostObject)
        : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)),
        pBody(pBody), btGhostObject(btGhostObject)
    {
    }

    btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
    {
        if (rayResult.m_collisionObject == pBody || rayResult.m_collisionObject == btGhostObject)
            return 1.0f;

        return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
    }
};

DynamicCharacterController::DynamicCharacterController(btSoftRigidDynamicsWorld* dynamicsWorld, btRigidBody* rigidBody)
    : btDynamicsWorld(dynamicsWorld), btRigidbody(rigidBody)
{
    // Keep upright
    btRigidbody->setAngularFactor(0.0f);
    // No sleeping (or else setLinearVelocity won't work)
    btRigidbody->setActivationState(DISABLE_DEACTIVATION);

    // Ghost object that is synchronized with rigid body
    btGhostObject = new btPairCachingGhostObject();
    btGhostObject->setCollisionShape(btRigidbody->getCollisionShape());
    btGhostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

    // Specify filters manually, otherwise ghost doesn't collide with statics for some reason
    btDynamicsWorld->addCollisionObject(btGhostObject, btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
}

DynamicCharacterController::~DynamicCharacterController()
{
    btDynamicsWorld->removeCollisionObject(btGhostObject);
    delete btGhostObject;
}

void DynamicCharacterController::Move(glm::vec2 direction)
{
    movementDirection += direction * directionMultiplier;
}

void DynamicCharacterController::Jump()
{
    jumpRequested = true;
}

void DynamicCharacterController::SetRotation(glm::quat rotation)
{
    btRigidbody->getWorldTransform().setRotation(PhysicsEngine::ToBtQuaternion(rotation));
}

void DynamicCharacterController::BeforeSimulationUpdate()
{
    if (jumpRequested && onGround && canJump)
    {
        auto suppressMovement = -btRigidbody->getLinearVelocity().y() / (btRigidbody->getLinearFactor().y() * btRigidbody->getInvMass());
        btRigidbody->applyCentralImpulse(btVector3(0.0f, jumpImpulse + suppressMovement, 0.0f));
        jumped = true;
        canJump = false;
        jumpDelay = 0.2f;
    }

    manualVelocity.y = btRigidbody->getLinearVelocity().getY();

    if (!jumped)
    {
        if (!onGround)
        {
            manualVelocity.y = glm::clamp(manualVelocity.y, std::numeric_limits<float>::lowest(), 0.0f);

            if (movementDirection.x != 0.0f || movementDirection.y != 0.0f)
            {
                auto position = btRigidbody->getWorldTransform().getOrigin();
                auto rayTestResultPosition = RayTestGroundOffset(position);

                auto directionForward = glm::normalize(movementDirection);
                auto rayTestResultForward = RayTestGroundOffset(btVector3(position.x() + directionForward.x * 0.04f, position.y(), position.z() + directionForward.y * 0.04f));

                if (rayTestResultPosition.first && rayTestResultForward.first && (rayTestResultForward.second > rayTestResultPosition.second))
                {
                    movementDirection.x = 0.0;
                    movementDirection.y = 0.0;
                }
            }
        }
        else
        {
            manualVelocity.y = glm::clamp(manualVelocity.y, std::numeric_limits<float>::lowest(), 0.0f);
            btRigidbody->setGravity(btVector3(0.0f, 0.0f, 0.0f));
        }
    }
    else
    {
        movementDirection.x = 0.0;
        movementDirection.y = 0.0;
    }
    if(jumped || !onGround)
    {
        btRigidbody->setGravity(btVector3(0.0f, -9.81f, 0.0f));
    }

    manualVelocity.x += movementDirection.x;
    manualVelocity.z += movementDirection.y;

    btRigidbody->setLinearVelocity(PhysicsEngine::ToBtVector3(manualVelocity));
}

void DynamicCharacterController::AfterSimulationUpdate()
{
    btGhostObject->setWorldTransform(btRigidbody->getWorldTransform());

    onGround = false;
    touching = false;

    ParseGhostContacts();
    UpdateVelocity();

    if (jumped)
        jumpDelay -= Engine::GetTime().FixedDeltaTime();

    if ((!onGround && !touching) || jumpDelay < 0)
        canJump = true;
    if ((onGround || touching) && canJump)
        jumped = false;

    movementDirection = glm::vec2(0.0);
    jumpRequested = false;
}

std::pair<bool, float> DynamicCharacterController::RayTestGroundOffset(btVector3 const& position)
{
    auto bottomYOffset = GetBottomYOffset();

    IgnoreBodyAndGhostCast rayCallback(btRigidbody, btGhostObject);
    btDynamicsWorld->rayTest(position, position - btVector3(0.0f, 2 * bottomYOffset, 0.0f), rayCallback);
    if (rayCallback.hasHit())
    {
        return { true, bottomYOffset - 2 * bottomYOffset * rayCallback.m_closestHitFraction };
    }

    return { false, 0.0f };
}

float DynamicCharacterController::GetBottomYOffset()
{
    auto capsuleRadius = reinterpret_cast<btCapsuleShape*>(btRigidbody->getCollisionShape())->getRadius();
    auto capsuleHalfHeight = reinterpret_cast<btCapsuleShape*>(btRigidbody->getCollisionShape())->getHalfHeight();

    return capsuleHalfHeight + capsuleRadius;
}

float DynamicCharacterController::GetBottomRoundedRegionYOffset()
{
    return reinterpret_cast<btCapsuleShape*>(btRigidbody->getCollisionShape())->getHalfHeight();
}

void DynamicCharacterController::ParseGhostContacts()
{
    btManifoldArray manifoldArray;
    btBroadphasePairArray &pairArray = btGhostObject->getOverlappingPairCache()->getOverlappingPairArray();
    int numPairs = pairArray.size();

    surfaceHitNormals.clear();

    for (int i = 0; i < numPairs; i++)
    {
        manifoldArray.clear();

        const btBroadphasePair &pair = pairArray[i];

        btBroadphasePair* collisionPair = btDynamicsWorld->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);

        if (collisionPair == NULL)
            continue;

        if (collisionPair->m_algorithm != NULL)
            collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

        for (int j = 0; j < manifoldArray.size(); j++)
        {
            btPersistentManifold* pManifold = manifoldArray[j];

            // Skip the rigid body the ghost monitors
            if (pManifold->getBody1() == btRigidbody)
                continue;

            for (int p = 0; p < pManifold->getNumContacts(); p++)
            {
                const btManifoldPoint &point = pManifold->getContactPoint(p);

                if (point.getDistance() <= 0.04f)
                {
                    const btVector3 &ptB = point.getPositionWorldOnB();

                    // If point is in rounded bottom region of capsule shape, it is on the ground
                    if (ptB.getY() >= (btRigidbody->getWorldTransform().getOrigin().getY() - GetBottomRoundedRegionYOffset()))
                    {
                        surfaceHitNormals.push_back(PhysicsEngine::ToGlmVec3(point.m_normalWorldOnB));
                    }
                    else
                    {
                        touching = true;
                    }
                }
            }
        }
    }
}

void DynamicCharacterController::UpdateVelocity()
{
    auto rayTestResult = RayTestGroundOffset(btRigidbody->getWorldTransform().getOrigin());
    if (rayTestResult.first && rayTestResult.second > -stepHeight)
    {
        onGround = true;
    }

    if (!jumped)
    {
        manualVelocity.x -= manualVelocity.x * deceleration * Engine::GetTime().FixedDeltaTime();
        manualVelocity.z -= manualVelocity.z * deceleration * Engine::GetTime().FixedDeltaTime();
    }

    for (size_t i = 0, size = surfaceHitNormals.size(); i < size; i++)
    {
        // Cancel velocity across normal
        glm::vec3 velInNormalDir = glm::proj(manualVelocity, surfaceHitNormals[i]);
        manualVelocity -= velInNormalDir * 1.05f;
    }
}

bool DynamicCharacterController::IsOnGround() const
{
    return onGround;
}