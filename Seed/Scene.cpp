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

void Scene::OnFixedUpdate()
{
}

void Scene::OnFrameUpdate()
{
    components.OnFrameUpdate();
}

void Scene::Render()
{
    components.Render();
}
