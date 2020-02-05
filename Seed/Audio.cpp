#include "Audio.h"

Audio* Audio::Clone()
{
    return new Audio(*this);
}
