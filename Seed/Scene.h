#pragma once

class SceneDefinition;
class Objects;
class Components;

class Scene
{
public:
    Scene(SceneDefinition& sceneDefinition,
          Objects& objects,
          Components& components);

    void Initialize(std::string scene);

	void OnFrameUpdate();
	void Render();
	void CleanObjects();

private:
    SceneDefinition& sceneDefinition;
    Objects& objects;
    Components& components;
};
