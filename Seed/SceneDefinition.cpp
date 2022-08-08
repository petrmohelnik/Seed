#include "SceneDefinition.h"
#include "Objects.h"
#include "Components.h"
#include "Engine.h"
#include "DefaultScene.h"
#include "HeightMapScene.h"

SceneDefinition::SceneDefinition()
    : objects(Engine::GetObjects())
    , fileSystem(Engine::GetFileSystem())
{
}

void SceneDefinition::InitializeScene(std::string scene)
{
    if (scene == "Default")
        DefaultScene(objects, fileSystem);
    else if (scene == "HeightMap")
        HeightMapScene(objects, fileSystem);
    else
        throw std::exception("Scene does not exist");
}
