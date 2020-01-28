#pragma once
#include "Object.h"

class BulletObject : public Object
{
public:
    using Object::Object;

    void Initialize() override;
    void SetPhysicsMaterial(float mass, float bounciness, float friction, float linearDamping, float angularDamping);
};

void BulletObject::Initialize()
{
    AddComponent<MeshRenderer>()->Load("rocket/rocket.gltf", false);
    AddComponent<MeshCollider>(GetComponent<MeshRenderer>()->GetSharedMesh(), false);
    GetComponent<Collider>()->SetMass(1.0f);
    GetComponent<Collider>()->SetBounciness(0.0f);
    //GetComponent<Collider>()->SetContinuousCollisionDetection(0.5f, 0.1f);
    GetComponent<Transform>()->SetScale(0.05f);
    GetComponent<Transform>()->RotateX(3.14f);
    AddComponent<Light>()->SetPointLight(0.5f, 0.5f);
    GetComponent<Light>()->SetShadowNearPlaneDistance(0.05f);
    GetComponent<Light>()->SetColor(glm::vec3(0.9f, 0.35f, 0.15f));

    //auto bulletMaterial = std::make_shared<Material>();
    //bulletMaterial->SetMetallicWorkflow();
    //bulletMaterial->Albedo->SetColor(glm::vec3(0.8f, 0.67f, 0.45f));
    //bulletMaterial->Metallic->SetColor(glm::vec3(1.0f, 0.4f, 1.0f));
    //GetComponent<MeshRenderer>()->SetMaterial(0, bulletMaterial);
}

void BulletObject::SetPhysicsMaterial(float mass, float bounciness, float friction, float linearDamping, float angularDamping)
{
    auto collider = GetComponent<Collider>();
    collider->SetMass(mass);
    collider->SetBounciness(bounciness);
    collider->SetFriction(friction);
    collider->SetLinearDamping(linearDamping);
    collider->SetAngularDamping(angularDamping);
}