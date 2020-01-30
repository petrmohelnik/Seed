#include "Scene.h"
#include "SceneDefinition.h"
#include "Objects.h"
#include "Components.h"
#include "Engine.h"

Scene::Scene(SceneDefinition& sceneDefinition)
    : sceneDefinition(sceneDefinition)
    , objects(Engine::GetObjects())
    , components(Engine::GetComponents())
{
}

void Scene::Initialize(std::string scene)
{
    sceneDefinition.InitializeScene(scene);
}

void Scene::OnCreateUpdate()
{
    components.OnCreateUpdate();
}

void Scene::OnFixedUpdate()
{
    components.OnFixedUpdate();
}

void Scene::OnFrameUpdate()
{
    components.OnFrameUpdate();
}

void Scene::SimulatePhysics()
{
    components.SimulatePhysics();
}

void Scene::ActivationUpdate()
{
    components.ActivationUpdate();
}

void Scene::Render()
{
    components.Render();
}
