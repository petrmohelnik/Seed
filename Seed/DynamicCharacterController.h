#pragma once

class DynamicCharacterController
{
public:
    DynamicCharacterController(btSoftRigidDynamicsWorld* pPhysicsWorld, btRigidBody* pRigidBody, float stepHeight);
    ~DynamicCharacterController();

    void Move(glm::vec2 dir);
    void Jump();
    
    void Update();

    glm::vec3 GetVelocity() const;
    bool IsOnGround() const;

private:
    void ParseGhostContacts();

    void UpdateVelocity();

    float GetBottomRoundedRegionYOffset();

    btSoftRigidDynamicsWorld* m_pDynamicsWorld = nullptr;
    btRigidBody* m_pRigidBody = nullptr;
    btPairCachingGhostObject* m_pGhostObject = nullptr;

    btTransform m_motionTransform;

    glm::vec3 m_manualVelocity = glm::vec3(0.0f);
    std::vector<glm::vec3> m_surfaceHitNormals;

    btVector3 m_previousPosition;

    bool m_onGround = false;
    bool m_hittingWall = false;

    float m_stepHeight;
    float m_deceleration = 10.0f;
    float m_jumpImpulse = 200.0f;
};
