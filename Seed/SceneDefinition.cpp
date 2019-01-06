#include "Objects.h"
#include "Components.h"
#include "SceneDefinition.h"
#include "DefaultScene.h"

SceneDefinition::SceneDefinition(Objects& objects,
                                 Components& components)
    : objects(objects)
    , components(components)
{
}

void SceneDefinition::InitializeScene(std::string scene)
{
    if (scene == "Default")
        DefaultScene(objects, components);
    else
        throw std::exception("Scene does not exist");
}
