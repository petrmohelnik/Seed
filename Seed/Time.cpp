#include "Time.h"

Uint32 Time::previousTime = 0;
Uint32 Time::currentTime = 0;

void Time::UpdateTime(Uint32 time)
{
	previousTime = currentTime;
	currentTime = time;
}

Uint32 Time::GetDelta()
{
	return currentTime - previousTime;
}
