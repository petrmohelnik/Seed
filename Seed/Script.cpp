#include "Script.h"
#include "Input.h"
#include "Time.h"
#include "Objects.h"

Script::Script(Input& input, 
               Time& time,               
               Objects& objects) 
    : input(input)
    , time(time)
    , objects(objects)
{
}
