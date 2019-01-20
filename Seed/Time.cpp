#include "Time.h"
#include "SDLWindow.h"

Uint32 Time::deltaTime = 0;
Uint32 Time::fixedDeltaTime = 20;
bool Time::paused = false;

Time::Time(SDLWindow& window) : window(window)
{
}

void Time::InitializeTime()
{
    currentTime = window.GetTime();
}

void Time::UpdateTime()
{
    if (!paused)
    {
        auto newTime = window.GetTime();
        deltaTime = newTime - currentTime;
        currentTime = newTime;
        fixedUpdateAccumulate += deltaTime;
    }
}

void Time::PauseTime()
{
    paused = true;
}

void Time::UnpauseTime()
{
    paused = false;
}

bool Time::StartFixedUpdate()
{
    if (fixedUpdateAccumulate < fixedDeltaTime)
        return false;

    fixedUpdateStart = window.GetTime();
    return true;
}

void Time::EndFixedUpdate()
{
    auto fixedUpdateDuration = window.GetTime() - fixedUpdateStart;
    if (fixedUpdateDuration < fixedDeltaTime)
        fixedUpdateAccumulate -= fixedDeltaTime;
    else
        fixedUpdateAccumulate -= fixedUpdateDuration;
}

Uint32 Time::DeltaTime()
{
    if (paused)
        return 0;
    return deltaTime;
}

Uint32 Time::FixedDeltaTime()
{
	return fixedDeltaTime;
}
