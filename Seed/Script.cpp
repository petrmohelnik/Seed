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
    , physics(physics)
    , object(object)
{
    transform = object->GetComponent<Transform>();
}
