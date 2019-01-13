#include "Scene.h"
#include "SceneDefinition.h"
#include "Objects.h"
#include "Components.h"

Scene::Scene(SceneDefinition& sceneDefinition,
             Objects& objects,
             Components& components)
    : sceneDefinition(sceneDefinition)
    , objects(objects)
    , components(components)
{
}

void Scene::Initialize(std::string scene)
{
    sceneDefinition.InitializeScene(scene);
}

void Scene::OnFrameUpdate()
{
    components.OnFrameUpdate();
}

void Scene::Render()
{
    components.Render();
}

void Scene::CleanObjects()
{
    objects.Destroy();
    components.CleanComponents();
}
