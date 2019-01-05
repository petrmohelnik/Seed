#pragma once

class Time
{
public:
	Time();

	void UpdateTime(Uint32 time);
	static Uint32 GetDeltaTime();

private:
	static Uint32 previousTime;
	static Uint32 currentTime;
};

