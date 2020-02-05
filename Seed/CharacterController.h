#pragma once
#include "CapsuleCollider.h"
#include "PhysicsObject.h"
#include "DynamicCharacterController.h"

class CharacterController : public CapsuleCollider, public PhysicsObject
{
public:
    CharacterController(Object* object, glm::vec3 center, float height, float radius);
    CharacterController(const CharacterController& characterController);

    void Move(glm::vec2 direction);
    void Jump();
    bool IsGrounded() const;

protected:
    friend class PhysicsEngine;

    void InitializePhysics() override;
    void BeforeSimulationUpdate() override;
    void AfterSimulationUpdate() override;

private:
    CharacterController* Clone() override;

    std::unique_ptr<DynamicCharacterController> dynamicCharacterController;
};

