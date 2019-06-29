#pragma once

class Object;

class Input
{
public:
	bool GameIsRunning();
	bool Pause();
	bool Resume();
	bool Key(SDL_Keycode key);
	bool KeyDown(SDL_Keycode key);
    bool MouseButton(Uint8 button); //SDL_BUTTON_RIGHT, SDL_BUTTON_LEFT
    bool MouseButtonDown(Uint8 button); //SDL_BUTTON_RIGHT, SDL_BUTTON_LEFT
    glm::ivec2 MouseMovement();
    void SliderFloat(const std::string& name, float& value, float min, float max);

protected:
    friend class SDLWindow;
	friend class Engine;

    void InitializeState();
	void AddInput(SDL_Event event);
	void ClearInputs();
	void CreateSceneGraph();

private:
	void CreateSceneGraphNode(Object* object);

	std::set<SDL_Keycode> keyHoldDown;
	std::set<SDL_Keycode> keyPressedDown;
	std::set<Uint8> mouseHoldDown;
	std::set<Uint8> mousePressedDown;
    glm::ivec2 mouseMovement;
    Uint32 mouseState;
    bool quit = false;
};

