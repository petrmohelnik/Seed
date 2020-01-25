#pragma once
#include "Script.h"

class LightScript : public Script
{
public:
    using Script::Script;

    void OnCollisionEnter(Collision const& collision) override;
    void OnMouseEnter() override;
    void OnMouseExit() override;
};

void LightScript::OnCollisionEnter(Collision const& collision)
{
    if (collision.OtherCollider->GetObject()->GetName() == "Bullet")
        if(auto light = object->GetComponent<Light>())
            light->Destroy();
}

inline void LightScript::OnMouseEnter()
{
    if (auto light = object->GetComponent<Light>())
        light->SetIntensity(light->GetIntensity() * 10.0f);
}

inline void LightScript::OnMouseExit()
{
    if (auto light = object->GetComponent<Light>())
        light->SetIntensity(light->GetIntensity() / 10.0f);
}
