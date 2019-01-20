#include "Time.h"

Uint32 Time::previousTime = 0;
Uint32 Time::currentTime = 0;

void Time::UpdateTime(Uint32 time)
{
	previousTime = currentTime;
	currentTime = time;
}

Uint32 Time::CurrentTime()
{
	return currentTime;
}

Uint32 Time::DeltaTime()
{
	return currentTime - previousTime;
}

Uint32 Time::FixedDeltaTime()
{
	return 20;
}
