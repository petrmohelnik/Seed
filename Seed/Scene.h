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

	void OnFixedUpdate();
	void OnFrameUpdate();
	void Render();

private:
    SceneDefinition& sceneDefinition;
    Objects& objects;
    Components& components;
};
