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

private:
    static Object* brokenDummy;
};
Object* TargetScript::brokenDummy = nullptr;

void TargetScript::OnCreate()
{
    if (!brokenDummy)
    {
        brokenDummy = fileSystem.LoadObjects("TargetDummyBroken/TargetDummyBroken.gltf", "TargetDummyBroken", false);
        brokenDummy->SetActive(false);
        for (auto& shard : brokenDummy->GetAllChildren())
        {
            auto mesh = shard->GetComponent<MeshRenderer>()->GetSharedMesh();
            mesh->DeleteDataAfterColliderLoad(true);
            shard->AddComponent<MeshCollider>(shard->GetComponent<MeshRenderer>()->GetSharedMesh(), true);
            shard->GetComponent<MeshCollider>()->SetMass(0.1f);
        }
    }
}

void TargetScript::OnCollisionEnter(Collision const& collision)
{
    if (collision.OtherCollider->GetObject()->GetName() == "Bullet")
    {
        auto clonedDummy = objects.CloneObject(brokenDummy);
        clonedDummy->SetActive(true);
        clonedDummy->GetComponent<Transform>()->Clone(object);
        for (auto& shard : clonedDummy->GetAllChildren())
        {
            shard->GetComponent<Collider>()->Destroy(5.0f);
        }
        object->Destroy();
    }
}
