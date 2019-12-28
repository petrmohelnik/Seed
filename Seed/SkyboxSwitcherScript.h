#pragma once
#include "Script.h"
#include "Engine.h"

class SkyboxSwitcherScript : public Script
{
public:
	using Script::Script;

	void Update() override;
	void AddSkybox(std::string path);

private:
	std::vector<std::string> paths;
};

void SkyboxSwitcherScript::Update()
{
	for (const auto& path : paths)
	{
		if(input.Button(path.c_str()))
			objects.SetSkybox(Engine::GetFileSystem().LoadCubeMapHDR(path));
	}
}

void SkyboxSwitcherScript::AddSkybox(std::string path)
{
	paths.push_back(std::move(path));
}
