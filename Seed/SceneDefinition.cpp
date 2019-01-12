#include "Objects.h"
#include "Components.h"
#include "SceneDefinition.h"
#include "DefaultScene.h"

SceneDefinition::SceneDefinition(Objects& objects)
    : objects(objects)
{
}

void SceneDefinition::InitializeScene(std::string scene)
{
    if (scene == "Default")
        DefaultScene(objects);
    else
        throw std::exception("Scene does not exist");
}
