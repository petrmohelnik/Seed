#include "Scene.h"
#include "Objects.h"
#include "Components.h"

Scene::Scene(Objects& objects,
             Components& components)
    : objects(objects)
    , components(components)
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

void Scene::CleanObjects()
{
    components.CleanComponents();
}
