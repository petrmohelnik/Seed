#include "DynamicCharacterController.h"
#include "PhysicsEngine.h"
#include "Engine.h"

class IgnoreBodyAndGhostCast :
    public btCollisionWorld::ClosestRayResultCallback
{
private:
    btRigidBody* m_pBody;
    btPairCachingGhostObject* m_pGhostObject;

public:
    IgnoreBodyAndGhostCast(btRigidBody* pBody, btPairCachingGhostObject* pGhostObject)
        : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)),
        m_pBody(pBody), m_pGhostObject(pGhostObject)
    {
    }

    btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
    {
        if (rayResult.m_collisionObject == m_pBody || rayResult.m_collisionObject == m_pGhostObject)
            return 1.0f;

        return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
    }
};

DynamicCharacterController::DynamicCharacterController(btSoftRigidDynamicsWorld* pPhysicsWorld, btRigidBody* pRigidBody)
    : m_pDynamicsWorld(pPhysicsWorld), m_pRigidBody(pRigidBody)
{
    // Keep upright
    m_pRigidBody->setAngularFactor(0.0f);
    // No sleeping (or else setLinearVelocity won't work)
    m_pRigidBody->setActivationState(DISABLE_DEACTIVATION);

    // Ghost object that is synchronized with rigid body
    m_pGhostObject = new btPairCachingGhostObject();

    m_pGhostObject->setCollisionShape(m_pRigidBody->getCollisionShape());
    m_pGhostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

    // Specify filters manually, otherwise ghost doesn't collide with statics for some reason
    m_pDynamicsWorld->addCollisionObject(m_pGhostObject, btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
}

DynamicCharacterController::~DynamicCharacterController()
{
    m_pDynamicsWorld->removeCollisionObject(m_pGhostObject);
    delete m_pGhostObject;
}

void DynamicCharacterController::Move(glm::vec2 dir)
{
    glm::vec2 velocityXZ(dir + glm::vec2(m_manualVelocity.x, m_manualVelocity.z));

    m_manualVelocity.x = velocityXZ.x;
    m_manualVelocity.z = velocityXZ.y;
}

void DynamicCharacterController::Jump()
{
    if (m_onGround && m_canJump)
    {
        m_pRigidBody->applyCentralImpulse(btVector3(0.0f, m_jumpImpulse, 0.0f));
        m_pRigidBody->setGravity(btVector3(0.0f, -9.81f, 0.0f));
        m_jumped = true;
        m_canJump = false;
    }
}

void DynamicCharacterController::SetRotation(glm::quat rotation)
{
    m_pRigidBody->getWorldTransform().setRotation(PhysicsEngine::ToBtQuaternion(rotation));
}

void DynamicCharacterController::Update()
{
    m_motionTransform = m_pRigidBody->getWorldTransform();
    m_pGhostObject->setWorldTransform(m_motionTransform);

    m_onGround = false;
    m_touching = false;

    ParseGhostContacts();
    UpdatePosition();
    UpdateVelocity();

    if (!m_onGround)
        m_canJump = true;
    if (m_onGround && m_canJump)
        m_jumped = false;
}

void DynamicCharacterController::ParseGhostContacts()
{
    btManifoldArray manifoldArray;
    btBroadphasePairArray &pairArray = m_pGhostObject->getOverlappingPairCache()->getOverlappingPairArray();
    int numPairs = pairArray.size();

    // Set false now, may be set true in test
    m_hittingWall = false;

    m_surfaceHitNormals.clear();

    for (int i = 0; i < numPairs; i++)
    {
        manifoldArray.clear();

        const btBroadphasePair &pair = pairArray[i];

        btBroadphasePair* collisionPair = m_pDynamicsWorld->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);

        if (collisionPair == NULL)
            continue;

        if (collisionPair->m_algorithm != NULL)
            collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

        for (int j = 0; j < manifoldArray.size(); j++)
        {
            btPersistentManifold* pManifold = manifoldArray[j];

            // Skip the rigid body the ghost monitors
            if (pManifold->getBody1() == m_pRigidBody)
                continue;

            for (int p = 0; p < pManifold->getNumContacts(); p++)
            {
                const btManifoldPoint &point = pManifold->getContactPoint(p);

                if (point.getDistance() <= 0.001f)
                {
                    const btVector3 &ptB = point.getPositionWorldOnB();

                    // If point is in rounded bottom region of capsule shape, it is on the ground
                    if (ptB.getY() < (m_motionTransform.getOrigin().getY() - GetBottomRoundedRegionYOffset()))
                    {
                        m_touching = true;
                    }
                    else
                    {
                        m_hittingWall = true;

                        m_surfaceHitNormals.push_back(PhysicsEngine::ToGlmVec3(point.m_normalWorldOnB));
                    }
                }
            }
        }
    }
}

void DynamicCharacterController::UpdatePosition()
{
    // Ray cast, ignore rigid body
    IgnoreBodyAndGhostCast rayCallBack_bottom(m_pRigidBody, m_pGhostObject);
    auto bottomYOffset = GetBottomYOffset();

    m_pDynamicsWorld->rayTest(m_pRigidBody->getWorldTransform().getOrigin(), m_pRigidBody->getWorldTransform().getOrigin() - btVector3(0.0f, bottomYOffset + m_stepHeight, 0.0f), rayCallBack_bottom);

    // Bump up if hit
    if (rayCallBack_bottom.hasHit())
    {
        //float previousY = m_pRigidBody->getWorldTransform().getOrigin().getY();

        //m_pRigidBody->getWorldTransform().getOrigin().setY(previousY + (bottomYOffset + m_stepHeight) * (1.0f - rayCallBack_bottom.m_closestHitFraction));

        //btVector3 vel(m_pRigidBody->getLinearVelocity());

        //vel.setY(0.0f);

        //m_pRigidBody->setLinearVelocity(vel);

        m_onGround = true;
    }

    float testOffset = 0.07f;

    // Ray cast, ignore rigid body
    IgnoreBodyAndGhostCast rayCallBack_top(m_pRigidBody, m_pGhostObject);

    m_pDynamicsWorld->rayTest(m_pRigidBody->getWorldTransform().getOrigin(), m_pRigidBody->getWorldTransform().getOrigin() + btVector3(0.0f, bottomYOffset + testOffset, 0.0f), rayCallBack_top);

    // Bump up if hit
    //if (rayCallBack_top.hasHit())
    //{
    //    m_pRigidBody->getWorldTransform().setOrigin(m_previousPosition);

    //    btVector3 vel(m_pRigidBody->getLinearVelocity());

    //    vel.setY(0.0f);

    //    m_pRigidBody->setLinearVelocity(vel);
    //}

    m_previousPosition = m_pRigidBody->getWorldTransform().getOrigin();
}

void DynamicCharacterController::UpdateVelocity()
{
    m_manualVelocity.y = m_pRigidBody->getLinearVelocity().getY();
    if ((!m_onGround) && !m_jumped)
    {
        m_manualVelocity.y = glm::clamp(m_manualVelocity.y, std::numeric_limits<float>::lowest(), 0.0f);
    }

    if ((m_onGround) && !m_jumped)
    {
        m_manualVelocity.y = 0.0f;
        m_pRigidBody->setGravity(btVector3(0.0f, 0.0f, 0.0f));
    }
    else
    {
        m_pRigidBody->setGravity(btVector3(0.0f, -9.81f, 0.0f));
    }

    m_pRigidBody->setLinearVelocity(PhysicsEngine::ToBtVector3(m_manualVelocity));

    // Decelerate
    m_manualVelocity.x -= m_manualVelocity.x * m_deceleration * Engine::GetTime().FixedDeltaTime();
    m_manualVelocity.z -= m_manualVelocity.z * m_deceleration * Engine::GetTime().FixedDeltaTime();

    if (m_hittingWall)
    {
        for (size_t i = 0, size = m_surfaceHitNormals.size(); i < size; i++)
        {
            // Cancel velocity across normal
            glm::vec3 velInNormalDir = glm::proj(m_manualVelocity, m_surfaceHitNormals[i]);

            // Apply correction
            m_manualVelocity -= velInNormalDir * 1.05f;
        }

        // Do not adjust rigid body velocity manually (so bodies can still be pushed by character)
        return;
    }
}

float DynamicCharacterController::GetBottomYOffset()
{
    auto capsuleRadius = reinterpret_cast<btCapsuleShape*>(m_pRigidBody->getCollisionShape())->getRadius();
    auto capsuleHalfHeight = reinterpret_cast<btCapsuleShape*>(m_pRigidBody->getCollisionShape())->getHalfHeight();

    return capsuleHalfHeight + capsuleRadius;
}

float DynamicCharacterController::GetBottomRoundedRegionYOffset()
{
    auto capsuleRadius = reinterpret_cast<btCapsuleShape*>(m_pRigidBody->getCollisionShape())->getRadius();
    auto capsuleHalfHeight = reinterpret_cast<btCapsuleShape*>(m_pRigidBody->getCollisionShape())->getHalfHeight();
    
    return capsuleHalfHeight + capsuleRadius / 2.0f;
}

glm::vec3 DynamicCharacterController::GetVelocity() const
{
    return PhysicsEngine::ToGlmVec3(m_pRigidBody->getLinearVelocity());
}

bool DynamicCharacterController::IsOnGround() const
{
    return m_onGround;
}