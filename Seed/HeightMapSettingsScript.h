#pragma once
#include "Script.h"
#include "BulletObject.h"

class HeightMapSettingsScript : public Script
{
private:
    inline HeightMapSettingsScript* Clone() { return new HeightMapSettingsScript(*this); }
public:
    using Script::Script;

    void OnCreate() override;
    void Update() override;

    float Scale = 0.5f;
    bool UseTessellation = false;
    std::string Name;

private:
    Object* heightMap = nullptr;
};

void HeightMapSettingsScript::OnCreate()
{
    heightMap = objects.GetObjectByName(Name);
}

inline void HeightMapSettingsScript::Update()
{
    input.PushWindow(Name + " settings");
    input.SliderFloat("Scale", Scale, 0.0f, 1.0f);
    if (UseTessellation)
        heightMap->GetComponent<MeshRenderer>()->GetSharedMaterial()->SetParallaxStrength(Scale);
    else
        heightMap->GetComponent<Transform>()->SetScale(glm::vec3(1.0f, Scale, 1.0f));
    input.PopWindow();
}
