#pragma once
#include "Script.h"
#include "BulletObject.h"

class PlayerScript : public Script
{
public:
    using Script::Script;

    void OnCreate() override;
    void FixedUpdate() override;
    void OnCollisionEnter(Collision const& collision) override;
    void OnCollisionStay(Collision const& collision) override;
    void OnCollisionExit(Collision const& collision) override;
    void Update() override;

    float RotationSensitivity = 0.003f;
    float MoveSensitivity = 30.0f;

private:
    Light* flashLight;
    Camera* camera;
    CharacterController* characterController;

    float verticalRotation = 0.0f;
};

void PlayerScript::OnCreate()
{
    camera = objects.GetObjectByName("PlayerCamera")->GetComponent<Camera>();
    flashLight = objects.GetObjectByName("FlashLight")->GetComponent<Light>();
    characterController = object->GetComponent<CharacterController>();
}

inline void PlayerScript::FixedUpdate()
{
    glm::vec3 moveDirection(0.0f);

    if (input.Key(SDLK_w))
        moveDirection += -transform->GetForwardAxis() * MoveSensitivity * time.FixedDeltaTime();
    if (input.Key(SDLK_s))
        moveDirection += transform->GetForwardAxis() * MoveSensitivity * time.FixedDeltaTime();
    if (input.Key(SDLK_d))
        moveDirection += transform->GetRightAxis() * MoveSensitivity * time.FixedDeltaTime();
    if (input.Key(SDLK_a))
        moveDirection += -transform->GetRightAxis() * MoveSensitivity * time.FixedDeltaTime();

    characterController->Move(glm::vec2(moveDirection.x, moveDirection.z));

    if (input.Key(SDLK_SPACE))
    {
        characterController->Jump();
    }
}

inline void PlayerScript::OnCollisionEnter(Collision const& collision)
{
    std::cout << "Enter: " + std::to_string(collision.ContactPoints[0].ContactDistance) << std::endl;
}

inline void PlayerScript::OnCollisionStay(Collision const& collision)
{
}

inline void PlayerScript::OnCollisionExit(Collision const& collision)
{
    std::cout << "Exit: " + std::to_string(collision.TotalImpulse) << std::endl;
}

void PlayerScript::Update()
{
    if (input.MouseButton(SDL_BUTTON_RIGHT))
    {
        transform->RotateY(-input.MouseDeltaPosition().x * RotationSensitivity, Transform::Space::World);

        verticalRotation += -input.MouseDeltaPosition().y * RotationSensitivity;
        verticalRotation = glm::clamp(verticalRotation, -0.9f, 0.9f);
        camera->GetTransform()->SetRotation(glm::quat(glm::vec3(verticalRotation, 0.0f, 0.0f)));
    }

    if (input.KeyDown(SDLK_f))
    {
        if (flashLight->GetIntensity() != 0.0)
            flashLight->SetIntensity(0.0);
        else
            flashLight->SetIntensity(100.0);
    }

    if (input.MouseButtonDown(SDL_BUTTON_LEFT))
    {
        auto bullet = objects.CreateObject<BulletObject>("Bullet");
        bullet->GetComponent<Transform>()->SetPosition(glm::vec3(transform->GetPosition()));
        bullet->GetComponent<Transform>()->TranslateY(1.0f);
        bullet->Destroy(15.0f);
    }
}
