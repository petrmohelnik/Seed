#pragma once

class Object;
class Component;

class Input
{
public:
    bool IsGameRunning();
    bool PauseRequest();
    bool ResumeRequest();
    bool Key(SDL_Keycode key);
    bool KeyDown(SDL_Keycode key);
    bool MouseButton(Uint8 button); //SDL_BUTTON_RIGHT, SDL_BUTTON_LEFT
    bool MouseButtonDown(Uint8 button); //SDL_BUTTON_RIGHT, SDL_BUTTON_LEFT
    glm::ivec2 MouseDeltaPosition();
    glm::ivec2 MousePosition();
    void SliderFloat(const std::string& name, float& value, float min, float max);
    void SliderFloatLog(const std::string& name, float& value, float min, float max, float power = 4.0f);
    void BulletText(const std::string& text);
    bool Button(const std::string& text);
    void PushWindow(const std::string& title, ImGuiWindowFlags flags = 0);
    void PopWindow();
    void InputFloat(const std::string& name, float& value, const std::function<void()>& onEnterPressed);

protected:
    friend class SDLWindow;
    friend class Engine;

    void InitializeState();
    void AddInput(SDL_Event event);
    void ClearInputs();
    void CreateSceneGraph();

private:
    void CreateSceneGraphNode(Object* object);
    std::string GetFullNameHash(Component* component, const std::string& name); //use to indenitfy imgui object when on the same position

    std::set<SDL_Keycode> keyHoldDown;
    std::set<SDL_Keycode> keyPressedDown;
    std::set<Uint8> mouseHoldDown;
    std::set<Uint8> mousePressedDown;
    glm::ivec2 mouseDeltaPosition;
    glm::ivec2 mousePosition;
    Uint32 mouseState;
    bool quit = false;
    bool paused = false;
};

