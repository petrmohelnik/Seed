#include "Script.h"
#include "Input.h"
#include "Time.h"
#include "Objects.h"

Script::Script(Object* object,
               Input& input,
               Time& time,               
               Objects& objects,
               PhysicsEngine& physics)
    : Component(object)
    , input(input)
    , time(time)
    , objects(objects)
    , physics(physics)
{
}
