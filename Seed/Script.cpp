#include "Script.h"
#include "Input.h"
#include "Time.h"
#include "Objects.h"

Script::Script(std::weak_ptr<Object> object,
               Input& input,
               Time& time,               
               Objects& objects) 
    : Component(object)
    , input(input)
    , time(time)
    , objects(objects)
{
}
