#include "stdafx.h"
#include "CharacterController.h"
#include "Transform.h"

CharacterController::CharacterController(Object* object, glm::vec3 center, float height, float radius) 
    : CapsuleCollider(object, center, height, radius)
{
    friction = 0.0;
    mass = 80.0f;
}

CharacterController::CharacterController(const CharacterController& characterController)
    : CapsuleCollider(characterController)
{
}

void CharacterController::Move(glm::vec2 direction)
{
    if(dynamicCharacterController)
        dynamicCharacterController->Move(direction);
}

void CharacterController::Jump()
{
    if (dynamicCharacterController)
        dynamicCharacterController->Jump();
}

bool CharacterController::IsGrounded() const
{
    if (dynamicCharacterController)
        return dynamicCharacterController->IsOnGround();
    else
        return false;
}

CharacterController* CharacterController::Clone()
{
    return new CharacterController(*this);
}

void CharacterController::InitializePhysics()
{
    dynamicCharacterController = std::make_unique<DynamicCharacterController>(btDynamicsWorld, btRigidbody);
}

void CharacterController::BeforeSimulationUpdate()
{
    dynamicCharacterController->SetRotation(GetTransform()->GetRotation());
    dynamicCharacterController->BeforeSimulationUpdate();
}

void CharacterController::AfterSimulationUpdate()
{
    dynamicCharacterController->AfterSimulationUpdate();
}
