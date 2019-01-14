#pragma once

class Time
{
public:
	static Uint32 GetDelta();

protected:
    friend class SDLWindow;
	void UpdateTime(Uint32 time);

private:
	static Uint32 previousTime;
    static Uint32 currentTime;
};

