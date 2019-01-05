#include "stdafx.h"
#include "Time.h"

Uint32 Time::previousTime = 0;
Uint32 Time::currentTime = 0;

Time::Time()
{
}

void Time::UpdateTime(Uint32 time)
{
	previousTime = currentTime;
	currentTime = time;
}

Uint32 Time::GetDeltaTime()
{
	return currentTime - previousTime;
}
