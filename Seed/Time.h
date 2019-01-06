#pragma once

class Time
{
public:
	Uint32 GetDeltaTime();

protected:
    friend class SDLWindow;
	void UpdateTime(Uint32 time);

private:
	Uint32 previousTime;
	Uint32 currentTime;
};

