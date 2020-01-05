#pragma once
#include "Object.h"

class BulletObject : public Object
{
public:
    using Object::Object;

    void Initialize() override;
};

void BulletObject::Initialize()
{
    AddComponent<SphereCollider>()->SetSphere(glm::vec3(0.0f, 0.0f, 0.0f), 0.1f);
    AddComponent<Rigidbody>();
    AddComponent<MeshRenderer>()->Load("sphere.obj");

    auto bulletMaterial = std::make_shared<Material>();
    bulletMaterial->SetMetallicWorkflow();
    bulletMaterial->Albedo->SetColor(glm::vec3(1.0f, 0.8f, 0.0f));
    bulletMaterial->Metallic->SetColor(glm::vec3(1.0f, 0.4f, 1.0f));
    GetComponent<MeshRenderer>()->SetMaterial(0, bulletMaterial);
}