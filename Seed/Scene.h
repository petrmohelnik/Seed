#pragma once

class SceneDefinition;
class Objects;
class Components;

class Scene
{
public:
    Scene(SceneDefinition& sceneDefinition);

    void Initialize(std::string scene);

    void OnCreateUpdate();
    void OnFixedUpdate();
    void OnFrameUpdate();
    void SimulatePhysics();
    void Render();

private:
    SceneDefinition& sceneDefinition;
    Objects& objects;
    Components& components;
};
