#include "Objects.h"
#include "Components.h"
#include "SceneDefinition.h"
#include "DefaultScene.h"

SceneDefinition::SceneDefinition(Objects& objects, FileSystem& fileSystem)
    : objects(objects), fileSystem(fileSystem)
{
}

void SceneDefinition::InitializeScene(std::string scene)
{
    if (scene == "Default")
        DefaultScene(objects, fileSystem);
    else
        throw std::exception("Scene does not exist");
}
