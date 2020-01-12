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

glm::vec3 CharacterController::GetVelocity() const
{
    if (dynamicCharacterController)
        return dynamicCharacterController->GetVelocity();
    else
        return glm::vec3(0.0f);
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
    dynamicCharacterController = std::make_unique<DynamicCharacterController>(btDynamicsWorld, btRigidbody, 0.0f);
}

void CharacterController::PhysicsUpdate()
{
    dynamicCharacterController->Update();
}
