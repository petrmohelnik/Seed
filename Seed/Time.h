#pragma once

class Time
{
public:
	static Uint32 DeltaTime();
	static Uint32 FixedDeltaTime();

protected:
    friend class SDLWindow;
	friend class Engine;

	void UpdateTime(Uint32 time);
	Uint32 CurrentTime();

private:
	static Uint32 previousTime;
    static Uint32 currentTime;
};

