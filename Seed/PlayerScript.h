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
    float MoveSensitivity = 1.0f;

private:
    Light* flashLight;
    Camera* camera;
    CharacterController* characterController;
    BulletObject* bulletDataHolder;

    float verticalRotation = 0.0f;
    float bulletForce = 0.0f;

    float bulletMass = 10.0f;
    float bulletBounciness = 0.0f;
    float bulletFriction = 0.5f;
    float bulletLinearDamping = 0.0f;
    float bulletAngularDamping = 0.0f;
};

void PlayerScript::OnCreate()
{
    camera = objects.GetObjectByName("PlayerCamera")->GetComponent<Camera>();
    flashLight = objects.GetObjectByName("FlashLight")->GetComponent<Light>();
    characterController = object->GetComponent<CharacterController>();

    auto scripts = object->GetComponents<Script>();

    bulletDataHolder = objects.CreateObject<BulletObject>("Bullet");
    bulletDataHolder->GetBody()->GetComponent<Collider>()->InitializeRigidbody();
    bulletDataHolder->SetActive(false);
}

inline void PlayerScript::FixedUpdate()
{
    glm::vec3 moveDirection(0.0f);

    if (input.Key(SDLK_w))
        moveDirection += transform->GetForwardAxis() * MoveSensitivity * time.FixedDeltaTime();
    if (input.Key(SDLK_s))
        moveDirection += -transform->GetForwardAxis() * MoveSensitivity * time.FixedDeltaTime();
    if (input.Key(SDLK_d))
        moveDirection += transform->GetRightAxis() * MoveSensitivity * time.FixedDeltaTime();
    if (input.Key(SDLK_a))
        moveDirection += -transform->GetRightAxis() * MoveSensitivity * time.FixedDeltaTime();

    characterController->Move(glm::vec2(moveDirection.x, moveDirection.z));

    if (input.Key(SDLK_SPACE))
    {
        characterController->Jump();
    }

    if (input.MouseButton(SDL_BUTTON_LEFT))
    {
        bulletForce += 200.0f * time.FixedDeltaTime();
    }
    else if (bulletForce != 0.0f)
    {
        auto bullet = objects.CreateObject<BulletObject>("Bullet");
        bullet->SetPhysicsMaterial(bulletMass, bulletBounciness, bulletFriction, bulletLinearDamping, bulletAngularDamping);


        bullet->GetComponent<Transform>()->SetRotation(camera->GetTransform()->GetRotation(), Transform::Space::World);
        bullet->GetComponent<Transform>()->RotateX(0.05f);
        auto initialPosition = camera->GetTransform()->GetPosition() + bullet->GetComponent<Transform>()->GetForwardAxis() * 0.25f;
        bullet->GetComponent<Transform>()->SetPosition(initialPosition);
        bullet->GetComponent<Transform>()->TranslateY(-0.1f);
        bullet->GetBody()->GetComponent<Collider>()->InitializeRigidbody();
        bullet->GetBody()->GetComponent<Collider>()->AddForce(bullet->GetComponent<Transform>()->GetForwardAxis() * bulletForce, Collider::ForceType::Impulse);
        bullet->Destroy(15.0f);
        bulletForce = 0.0f;
    }

    if (input.MouseButton(SDL_BUTTON_MIDDLE))
    {
        PhysicsEngine::RaycastHit hit;
        input.MousePosition();
        auto fromPos = camera->ScreenPositionToWorld(glm::vec3(input.MousePosition(), camera->GetNearPlane()));
        auto toPos = camera->ScreenPositionToWorld(glm::vec3(input.MousePosition(), camera->GetFarPlane()));
        auto rayDirection = glm::normalize(toPos - fromPos);
        if (physics.Raycast(fromPos, rayDirection, hit, camera->GetFarPlane() - camera->GetNearPlane()))
        {
            hit.Collider->AddForceAtPosition(rayDirection * 100.0f, Collider::ForceType::Force, hit.Point);
        }
    }

    if (input.Key(SDLK_e))
    {
        PhysicsEngine::RaycastHit hit;
        auto rayDirection = camera->GetTransform()->GetForwardAxis();
        auto cameraPosition = camera->GetTransform()->GetPosition();
        if (physics.Raycast(cameraPosition, rayDirection, hit))
        {
            hit.Collider->AddForce((cameraPosition - hit.Point) * 1.0f, Collider::ForceType::Acceleration);
        }
    }
}

inline void PlayerScript::OnCollisionEnter(Collision const& collision)
{
}

inline void PlayerScript::OnCollisionStay(Collision const& collision)
{
}

inline void PlayerScript::OnCollisionExit(Collision const& collision)
{
}

void PlayerScript::Update()
{
    if (input.MouseButton(SDL_BUTTON_RIGHT))
    {
        transform->RotateY(-input.MouseDeltaPosition().x * RotationSensitivity, Transform::Space::World);

        verticalRotation += -input.MouseDeltaPosition().y * RotationSensitivity;
        verticalRotation = glm::clamp(verticalRotation, -1.0f, 1.0f);
        camera->GetTransform()->SetRotation(glm::quat(glm::vec3(verticalRotation, 0.0f, 0.0f)));
    }

    if (input.KeyDown(SDLK_f))
    {
        if (flashLight->GetIntensity() != 0.0)
            flashLight->SetIntensity(0.0);
        else
            flashLight->SetIntensity(100.0);
    }

    input.PushWindow("Bullet attributes");
    input.SliderFloat("bulletMass", bulletMass, 0.0f, 10.0f);
    input.SliderFloat("bulletBounciness", bulletBounciness, 0.0f, 1.0f);
    input.SliderFloat("bulletFriction", bulletFriction, 0.0f, 1.0f);
    input.SliderFloat("bulletLinearDamping", bulletLinearDamping, 0.0f, 1.0f);
    input.SliderFloat("bulletAngularDamping", bulletAngularDamping, 0.0f, 1.0f);
    input.PopWindow();
}
