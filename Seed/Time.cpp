#include "Time.h"

void Time::UpdateTime(Uint32 time)
{
	previousTime = currentTime;
	currentTime = time;
}

Uint32 Time::GetDeltaTime()
{
	return currentTime - previousTime;
}
