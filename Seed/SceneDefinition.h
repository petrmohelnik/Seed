#pragma once
#include "Objects.h"
#include "Components.h"
#include "Object.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "Audio.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Script.h"
#include "FileSystem.h"

class SceneDefinition
{
public:
    SceneDefinition(Objects& objects);

    void InitializeScene(std::string scene);

private:
    Objects& objects;
};
