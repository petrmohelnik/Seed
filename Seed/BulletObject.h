#pragma once
#include "Object.h"

class BulletObject : public Object
{
public:
    using Object::Object;

    void Initialize() override;
    Object* GetBody();
    void SetPhysicsMaterial(float mass, float bounciness, float friction, float linearDamping, float angularDamping);
};

void BulletObject::Initialize()
{
    auto bulletBody = objects.CreateObject("Bullet");
    bulletBody->AddComponent<MeshRenderer>()->Load("rocket/rocket.gltf", false);
    bulletBody->AddComponent<MeshCollider>(bulletBody->GetComponent<MeshRenderer>()->GetSharedMesh(), true);
    bulletBody->GetComponent<MeshRenderer>()->GetSharedMesh()->DeleteDataAfterColliderLoad(true);
    bulletBody->GetComponent<Collider>()->SetMass(5.0f);
    bulletBody->GetComponent<Collider>()->SetBounciness(0.0f);
    bulletBody->GetComponent<Collider>()->SetContinuousCollisionDetection(0.1f, 0.05f);
    bulletBody->GetComponent<Transform>()->TranslateZ(-0.35f);
    bulletBody->GetComponent<Transform>()->SetParent(this);
    auto bulletLight = objects.CreateObject("BulletLight");
    bulletLight->AddComponent<Light>()->SetPointLight(0.3f, 0.3f);
    bulletLight->GetComponent<Light>()->SetShadowNearPlaneDistance(0.06f);
    bulletLight->GetComponent<Light>()->SetColor(glm::vec3(0.9f, 0.35f, 0.15f));
    bulletLight->GetComponent<Transform>()->TranslateZ(0.33f);
    bulletLight->GetComponent<Transform>()->SetParent(bulletBody);
}

Object* BulletObject::GetBody()
{
    return GetComponent<Transform>()->GetChild(0)->GetObject();
}

void BulletObject::SetPhysicsMaterial(float mass, float bounciness, float friction, float linearDamping, float angularDamping)
{
    auto collider = GetBody()->GetComponent<Collider>();
    collider->SetMass(mass);
    collider->SetBounciness(bounciness);
    collider->SetFriction(friction);
    collider->SetLinearDamping(linearDamping);
    collider->SetAngularDamping(angularDamping);
}