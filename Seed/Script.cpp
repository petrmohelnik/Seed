#include "Script.h"
#include "Input.h"
#include "Time.h"
#include "Objects.h"
#include "Engine.h"

Script::Script(Object* object, PhysicsEngine& physics)
    : Component(object)
    , input(Engine::GetInput())
    , time(Engine::GetTime())
    , objects(Engine::GetObjects())
    , fileSystem(Engine::GetFileSystem())
    , physics(physics)
{
    transform = object->GetComponent<Transform>();
}

Script::Script(Script& script) 
    : Component(script)
    , input(script.input)
    , time(script.time)
    , objects(script.objects)
    , fileSystem(script.fileSystem)
    , physics(script.physics)
{
}
