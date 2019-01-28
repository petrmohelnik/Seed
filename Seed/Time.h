#pragma once
class SDLWindow;

class Time
{
public:
    Time(SDLWindow& window);

	Uint32 DeltaTime();
	Uint32 FixedDeltaTime();

protected:
	friend class Engine;

    void InitializeTime();
	void UpdateTime();
    void PauseTime();
    void UnpauseTime();
    bool StartFixedUpdate();
    void EndFixedUpdate();

private:
    Uint32 deltaTime = 0;
    Uint32 fixedDeltaTime = 20;
    Uint32 currentTime = 0;
    Uint32 fixedUpdateAccumulate = 0;
    Uint32 fixedUpdateStart = 0;
    bool paused = false;

    SDLWindow& window;
};

