#pragma once
class SDLWindow;

class Time
{
public:
    Time(SDLWindow& window);

    float DeltaTime();
    float FixedDeltaTime();

protected:
	friend class Engine;

    void InitializeTime();
	void UpdateTime();
    void PauseTime();
    void UnpauseTime();
    bool StartFixedUpdate();
    void EndFixedUpdate();

private:
    double deltaTime = 0;
    double fixedDeltaTime = 0.02;
    double currentTime = 0;
    double fixedUpdateAccumulate = 0;
    double fixedUpdateStart = 0;
    bool paused = false;

    SDLWindow& window;
};

