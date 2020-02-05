#pragma once
#include "Script.h"
#include "Engine.h"

class SkyboxSwitcherScript : public Script
{
private:
    inline SkyboxSwitcherScript* Clone() { return new SkyboxSwitcherScript(*this); }
public:
    using Script::Script;

    void Update() override;
    void AddSkybox(std::string path);

private:
    std::vector<std::string> paths;
};

void SkyboxSwitcherScript::Update()
{
    input.PushWindow("Switch skybox");
    for (const auto& path : paths)
    {
        if (input.Button(path.c_str()))
        {
            objects.RemoveSkybox();
            objects.SetSkybox(Engine::GetFileSystem().LoadCubeMapHDR(path));
        }
    }
    input.PopWindow();
}

void SkyboxSwitcherScript::AddSkybox(std::string path)
{
    paths.push_back(std::move(path));
}
