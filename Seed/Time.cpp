#include "Time.h"
#include "SDLWindow.h"

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
    currentTime = window.GetTime();
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


float Time::DeltaTime()
{
    if (paused)
        return 0;
    return static_cast<float>(deltaTime);
}

float Time::FixedDeltaTime()
{
    return static_cast<float>(fixedDeltaTime);
}
