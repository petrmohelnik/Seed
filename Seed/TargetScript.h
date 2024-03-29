#pragma once
#include "Script.h"

class TargetScript : public Script
{
private:
    inline TargetScript* Clone() { return new TargetScript(*this); }
public:
    using Script::Script;

    void OnCreate() override;
    void OnCollisionEnter(Collision const& collision) override;

    std::string TargetPath;
    std::string TargetName;

private:
    static std::unordered_map<std::string, Object*> brokenDummies;
};
std::unordered_map<std::string, Object*> TargetScript::brokenDummies;

void TargetScript::OnCreate()
{
    if (brokenDummies.find(TargetPath) == std::end(brokenDummies))
    {
        auto brokenDummy = fileSystem.LoadObjects(TargetPath, TargetName, false);
        brokenDummy->SetActive(false);
        for (auto& shard : brokenDummy->GetAllChildren())
        {
            auto mesh = shard->GetComponent<MeshRenderer>()->GetSharedMesh();
            mesh->DeleteDataAfterColliderLoad(true);
            shard->GetComponent<MeshRenderer>()->SetCastsShadow(false);
            shard->AddComponent<MeshCollider>(shard->GetComponent<MeshRenderer>()->GetSharedMesh(), true);
            shard->GetComponent<MeshCollider>()->SetMass(object->GetComponent<Collider>()->GetMass() / brokenDummy->GetComponent<Transform>()->GetChildCount());
            shard->GetComponent<MeshCollider>()->SetFriction(object->GetComponent<Collider>()->GetFriction());
            shard->GetComponent<MeshCollider>()->SetBounciness(object->GetComponent<Collider>()->GetBounciness());
        }
        brokenDummies.insert({ TargetPath, brokenDummy });
    }
}

void TargetScript::OnCollisionEnter(Collision const& collision)
{
    if (collision.OtherCollider->GetObject()->GetName() == "Bullet")
    {
        auto clonedDummy = objects.CloneObject(brokenDummies[TargetPath]);
        clonedDummy->SetActive(true);
        clonedDummy->GetComponent<Transform>()->Clone(object);
        for (auto& shard : clonedDummy->GetAllChildren())
        {
            auto explosionDirection = glm::normalize(shard->GetComponent<Transform>()->GetPosition() - collision.ContactPoints[0].Point);
            shard->GetComponent<Collider>()->InitializeRigidbody();
            shard->GetComponent<Collider>()->AddForce(explosionDirection * collision.TotalImpulse * collision.TotalImpulse * 0.0001f, Collider::ForceType::Impulse);
        }

        object->Destroy();
    }
}
