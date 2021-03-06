#include "Input.h"
#include "Object.h"
#include "Component.h"
#include "Identifiable.h"
#include "Engine.h"

bool Input::IsGameRunning()
{
    return !quit;
}

bool Input::PauseRequest()
{
    if (!paused)
    {
        paused = KeyDown(SDLK_LSHIFT);
        return paused;
    }
    return false;
}

bool Input::ResumeRequest()
{
    if (paused)
    {
        paused = !KeyDown(SDLK_LSHIFT);
        return !paused;
    }
    return false;
}

void Input::InitializeState()
{
    mouseState = SDL_GetMouseState(nullptr, nullptr);
}

void Input::AddInput(SDL_Event event)
{
    switch (event.type)
    {
    case SDL_QUIT:
        quit = true;
    case SDL_KEYDOWN:
        if(!Key(event.key.keysym.sym))
            keyPressedDown.insert(event.key.keysym.sym);
        keyHoldDown.insert(event.key.keysym.sym);
        break;
    case SDL_KEYUP:
        keyHoldDown.erase(event.key.keysym.sym);
        break;
    case SDL_MOUSEBUTTONDOWN:
        if (!MouseButton(event.button.button))
            mousePressedDown.insert(event.button.button);
        mouseHoldDown.insert(event.button.button);
        break;
    case SDL_MOUSEBUTTONUP:
        mouseHoldDown.erase(event.button.button);
        break;
    case SDL_MOUSEMOTION:
        mouseDeltaPosition = glm::ivec2(event.motion.xrel, event.motion.yrel);
        mousePosition = glm::ivec2(event.motion.x, event.motion.y);
        break;
    }
}

void Input::ClearInputs()
{
    keyPressedDown.clear();
    mousePressedDown.clear();
    mouseDeltaPosition = glm::ivec2(0, 0);
}

bool Input::Key(SDL_Keycode key)
{
    return keyHoldDown.find(key) != std::end(keyHoldDown);
}

bool Input::KeyDown(SDL_Keycode key)
{
    return keyPressedDown.find(key) != std::end(keyPressedDown);
}

bool Input::MouseButton(Uint8 button)
{
    return mouseHoldDown.find(button) != std::end(mouseHoldDown);//mouseState & SDL_BUTTON(button);
}

bool Input::MouseButtonDown(Uint8 button)
{
    return mousePressedDown.find(button) != std::end(mousePressedDown);
}

glm::ivec2 Input::MouseDeltaPosition()
{
    return mouseDeltaPosition;
}

glm::ivec2 Input::MousePosition()
{
    return mousePosition;
}

void Input::SliderFloat(const std::string& name, float& value, float min, float max)
{
    ImGui::SliderFloat(name.c_str(), &value, min, max);
}

void Input::SliderFloatLog(const std::string& name, float& value, float min, float max, float power)
{
    ImGui::SliderFloat(name.c_str(), &value, min, max, "%.3f", power);
}

void Input::SliderFloat3(const std::string& name, glm::vec3& value, float min, float max)
{
    ImGui::SliderFloat3(name.c_str(), glm::value_ptr(value), min, max);
}

void Input::Text(const std::string& text)
{
    ImGui::Text(text.c_str());
}

void Input::BulletText(const std::string& text)
{
    ImGui::BulletText(text.c_str());
}

bool Input::Button(const std::string& text)
{
    return ImGui::Button(text.c_str());
}

bool Input::CheckBox(const std::string& name, bool& value)
{
    return ImGui::Checkbox(name.c_str(), &value);
}

void Input::PushWindow(const std::string& title, ImGuiWindowFlags flags)
{
    ImGui::Begin(title.c_str(), nullptr, flags);
}

void Input::PopWindow()
{
    ImGui::End();
}

void Input::InputFloat(const std::string& name, float& value, const std::function<void()>& onEnterPressed)
{
    if(ImGui::InputFloat(name.c_str(), &value, 0.0f, 0.0f, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
        onEnterPressed();
}

bool Input::ColorPicker(const std::string& name, glm::vec3& value)
{
    return ImGui::ColorEdit3(name.c_str(), glm::value_ptr(value));
}

void Input::CreateSceneGraph()
{
    //ImGui::ShowDemoWindow();

    PushWindow("Scene graph");
    auto root = Engine::GetComponents().GetRoot()->GetComponent<Transform>();
    CreateSceneGraphNode(root->GetObject());
    PopWindow();
}

void Input::CreateSceneGraphNode(Object* object)
{
    auto transform = object->GetComponent<Transform>();
    if (ImGui::TreeNode(GetFullNameHash(object, object->GetName()).c_str()))
    {
        for (int index = 0; index < transform->GetChildCount(); index++)
            CreateSceneGraphNode(transform->GetChild(index)->GetObject());
        for (const auto& component : object->GetAllComponents())
            component->OnInputGraphUpdate();
        ImGui::TreePop();
    }
}

std::string Input::GetFullNameHash(Identifiable* object, const std::string& name)
{
    return name + "##" + std::to_string(object->GetUniqueId());
}
