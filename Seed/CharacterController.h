#pragma once
#include "CapsuleCollider.h"
#include "PhysicsObject.h"
#include "DynamicCharacterController.h"

class CharacterController : public CapsuleCollider, public PhysicsObject
{
public:
    CharacterController(Object* object);

    void Move(glm::vec2 direction);
    void Jump();
    glm::vec3 GetVelocity() const;
    bool IsGrounded() const;

protected:
    friend class PhysicsEngine;

    void InitializePhysics() override;
    void BeforeSimulationUpdate() override;
    void AfterSimulationUpdate() override;

private:
    std::unique_ptr<DynamicCharacterController> dynamicCharacterController;
};
