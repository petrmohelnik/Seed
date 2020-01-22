#pragma once

class DynamicCharacterController
{
public:
    DynamicCharacterController(btSoftRigidDynamicsWorld* physicsWorld, btRigidBody* rigidBody);
    ~DynamicCharacterController();

    void Move(glm::vec2 direction);
    void Jump();
    void SetRotation(glm::quat rotation);
    
    void BeforeSimulationUpdate();
    void AfterSimulationUpdate();

    bool IsOnGround() const;

private:
    std::pair<bool, float> RayTestGroundOffset(btVector3 const& position);
    float GetBottomYOffset();
    float GetBottomRoundedRegionYOffset();

    void ParseGhostContacts();
    void UpdateVelocity();

    btSoftRigidDynamicsWorld* btDynamicsWorld = nullptr;
    btRigidBody* btRigidbody = nullptr;
    btPairCachingGhostObject* btGhostObject = nullptr;

    glm::vec3 manualVelocity = glm::vec3(0.0f);
    glm::vec2 jumpManualVelocity = glm::vec2(0.0f);
    std::vector<glm::vec3> surfaceHitNormals;

    bool onGround = false;
    bool touching = false;
    bool canJump = true;
    bool jumped = false;

    float stepHeight = 0.08f;
    float deceleration = 10.0f;
    float jumpImpulse = 250.0f;
    float directionMultiplier = 30.0f;
    
    glm::vec2 movementDirection = glm::vec2(0.0f);
    bool jumpRequested = false;
    float jumpDelay = 0.0f;
};
