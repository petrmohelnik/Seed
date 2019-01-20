#pragma once
class SDLWindow;

class Time
{
public:
    Time(SDLWindow& window);

	static Uint32 DeltaTime();
	static Uint32 FixedDeltaTime();

protected:
	friend class Engine;

    void InitializeTime();
	void UpdateTime();
    void PauseTime();
    void UnpauseTime();
    bool StartFixedUpdate();
    void EndFixedUpdate();

private:
    static Uint32 deltaTime;
    static Uint32 fixedDeltaTime;
    Uint32 currentTime;
    Uint32 fixedUpdateAccumulate = 0;
    Uint32 fixedUpdateStart = 0;
    static bool paused;

    SDLWindow& window;
};

