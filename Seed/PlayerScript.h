#pragma once
#include "Script.h"
#include "BulletObject.h"

class PlayerScript : public Script
{
private:
    inline PlayerScript* Clone() { return new PlayerScript(*this); }
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
    Object* chosenBullet = nullptr;

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
        Transform* bulletTransform;
        Collider* bulletCollider;
        if (chosenBullet)
        {
            auto bullet = objects.CloneObject(chosenBullet);
            bulletTransform = bullet->GetComponent<Transform>();
            bulletCollider = bullet->GetComponent<Collider>();
        }
        else
        {
            auto bullet = objects.CreateObject<BulletObject>("Bullet");
            bullet->SetPhysicsMaterial(bulletMass, bulletBounciness, bulletFriction, bulletLinearDamping, bulletAngularDamping);
            bulletTransform = bullet->GetComponent<Transform>();
            bulletCollider = bullet->GetBody()->GetComponent<Collider>();
        }
        
        bulletTransform->SetRotation(camera->GetTransform()->GetRotation(), Transform::Space::World);
        bulletTransform->RotateX(0.05f);
        auto initialPosition = camera->GetTransform()->GetPosition() + bulletTransform->GetForwardAxis() * 0.25f;
        bulletTransform->SetPosition(initialPosition);
        bulletTransform->TranslateY(-0.1f);
        bulletCollider->InitializeRigidbody();
        bulletCollider->AddForce(bulletTransform->GetForwardAxis() * bulletForce, Collider::ForceType::Impulse);
        bulletTransform->GetObject()->Destroy(15.0f);

        bulletForce = 0.0f;
    }

    if (input.MouseButton(SDL_BUTTON_MIDDLE))
    {
        PhysicsEngine::RaycastHit hit;
        glm::vec3 fromPos;
        auto rayDirection = camera->RayDirectionFromScreenPosition(input.MousePosition(), fromPos);
        if (physics.Raycast(fromPos, rayDirection, hit, camera->GetFarPlane() - camera->GetNearPlane()) && hit.Collider->IsRigidbodyInitialized())
        {
            hit.Collider->AddForceAtPosition(rayDirection * 40.0f, Collider::ForceType::Acceleration, hit.Point);
        }
    }

    if (input.Key(SDLK_e))
    {
        PhysicsEngine::RaycastHit hit;
        auto rayDirection = camera->GetTransform()->GetForwardAxis();
        auto upDirection = camera->GetTransform()->GetUpAxis();
        auto rightDirection = camera->GetTransform()->GetRightAxis();
        auto cameraPosition = camera->GetTransform()->GetPosition();
        if (physics.Raycast(cameraPosition, rayDirection, hit) && hit.Collider->IsRigidbodyInitialized())
            hit.Collider->AddForce((cameraPosition - hit.Point) * 1.0f, Collider::ForceType::Acceleration);
        if (physics.Raycast(cameraPosition + upDirection * 0.3f, rayDirection, hit) && hit.Collider->IsRigidbodyInitialized())
            hit.Collider->AddForce((cameraPosition - hit.Point) * 1.0f, Collider::ForceType::Acceleration);
        if (physics.Raycast(cameraPosition - upDirection * 0.3f, rayDirection, hit) && hit.Collider->IsRigidbodyInitialized())
            hit.Collider->AddForce((cameraPosition - hit.Point) * 1.0f, Collider::ForceType::Acceleration);
        if (physics.Raycast(cameraPosition + rightDirection * 0.3f, rayDirection, hit) && hit.Collider->IsRigidbodyInitialized())
            hit.Collider->AddForce((cameraPosition - hit.Point) * 1.0f, Collider::ForceType::Acceleration);
        if (physics.Raycast(cameraPosition - rightDirection * 0.3f, rayDirection, hit) && hit.Collider->IsRigidbodyInitialized())
            hit.Collider->AddForce((cameraPosition - hit.Point) * 1.0f, Collider::ForceType::Acceleration);
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

    if (input.KeyDown(SDLK_r))
    {
        PhysicsEngine::RaycastHit hit;
        glm::vec3 fromPos;
        auto rayDirection = camera->RayDirectionFromScreenPosition(input.MousePosition(), fromPos);
        if (physics.Raycast(fromPos, rayDirection, hit, camera->GetFarPlane() - camera->GetNearPlane()))
        {
            if (hit.Collider->GetMass() != 0.0f)
            {
                chosenBullet = hit.Collider->GetObject();
            }
        }
    }
    if (input.KeyDown(SDLK_t))
    {
        chosenBullet = nullptr;
    }

    input.PushWindow("Bullet attributes");
    input.SliderFloat("Mass", bulletMass, 0.0f, 10.0f);
    input.SliderFloat("Bounciness", bulletBounciness, 0.0f, 1.0f);
    input.SliderFloat("Friction", bulletFriction, 0.0f, 1.0f);
    input.SliderFloat("LinearDamping", bulletLinearDamping, 0.0f, 1.0f);
    input.SliderFloat("AngularDamping", bulletAngularDamping, 0.0f, 1.0f);
    input.PopWindow();
}
