#include "stdafx.h"
#include "CharacterController.h"
#include "Transform.h"

CharacterController::CharacterController(Object* object) : CapsuleCollider(object)
{
    friction = 0.0;
    mass = 80.0f;
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
