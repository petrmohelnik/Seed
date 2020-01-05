#pragma once
#include "Script.h"
#include "BulletObject.h"

class PlayerScript : public Script
{
public:
    using Script::Script;

    void OnCreate() override;
    void Update() override;

    float RotationSensitivity = 0.003f;
    float MoveSensitivity = 5.0f;

private:
    Light* flashLight;
    Camera* camera;

    float verticalRotation = 0.0f;
};

void PlayerScript::OnCreate()
{
    camera = objects.GetObjectByName("PlayerCamera")->GetComponent<Camera>();
    flashLight = objects.GetObjectByName("FlashLight")->GetComponent<Light>();
}

void PlayerScript::Update()
{
    if (input.Key(SDLK_w))
        transform->Translate(-transform->GetForwardAxis() * MoveSensitivity * time.DeltaTime(), Transform::Space::World);
    if (input.Key(SDLK_s))
        transform->Translate(transform->GetForwardAxis() * MoveSensitivity * time.DeltaTime(), Transform::Space::World);
    if (input.Key(SDLK_d))
        transform->Translate(transform->GetRightAxis() * MoveSensitivity * time.DeltaTime(), Transform::Space::World);
    if (input.Key(SDLK_a))
        transform->Translate(-transform->GetRightAxis() * MoveSensitivity * time.DeltaTime(), Transform::Space::World);

    if (input.MouseButton(SDL_BUTTON_RIGHT))
    {
        transform->RotateY(-input.MouseDeltaPosition().x * RotationSensitivity, Transform::Space::World);

        verticalRotation += -input.MouseDeltaPosition().y * RotationSensitivity;
        verticalRotation = glm::clamp(verticalRotation, -0.8f, 0.8f);
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
        bullet->GetComponent<Transform>()->TranslateY(10.0f);
        bullet->Destroy(3.0f);
    }
}
