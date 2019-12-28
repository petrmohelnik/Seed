#include "Input.h"
#include "Object.h"
#include "Component.h"
#include "Engine.h"

bool Input::IsGameRunning()
{
	return !quit;
}

bool Input::Pause()
{
	return false;
}

bool Input::Resume()
{
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
        keyPressedDown.insert(event.key.keysym.sym);
        keyHoldDown.insert(event.key.keysym.sym);
		break;
    case SDL_KEYUP:
        keyHoldDown.erase(event.key.keysym.sym);
        break;
    case SDL_MOUSEBUTTONDOWN:
        mousePressedDown.insert(event.button.button);
        mouseHoldDown.insert(event.button.button);
        break;
    case SDL_MOUSEBUTTONUP:
        mouseHoldDown.erase(event.button.button);
        break;
    case SDL_MOUSEMOTION:
        mouseMovement = glm::ivec2(event.motion.xrel, event.motion.yrel);
        break;
	}
}

void Input::ClearInputs()
{
    keyPressedDown.clear();
    mousePressedDown.clear();
    mouseMovement = glm::ivec2(0, 0);
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

glm::ivec2 Input::MouseMovement()
{
    return mouseMovement;
}

void Input::SliderFloat(const std::string& name, float& value, float min, float max)
{
    ImGui::SliderFloat(name.c_str(), &value, min, max);
}

void Input::SliderFloatLog(const std::string& name, float& value, float min, float max, float power)
{
    ImGui::SliderFloat(name.c_str(), &value, min, max, "%.3f", power);
}

void Input::BulletText(const std::string& text)
{
    ImGui::BulletText(text.c_str());
}

bool Input::Button(const std::string& text)
{
	return ImGui::Button(text.c_str());
}

void Input::CreateSceneGraph()
{
	ImGui::ShowDemoWindow();

	auto root = Engine::GetComponents().GetRoot()->GetComponent<Transform>();
	CreateSceneGraphNode(root->GetObject());
}

void Input::CreateSceneGraphNode(Object* object)
{
	auto transform = object->GetComponent<Transform>();
    if (object->GetComponent<Transform>()->GetChildCount() == 0)
    {
        if (ImGui::TreeNode(object->GetName().c_str()))
        {
            for (const auto& component : object->GetAllComponents())
                component->OnInputGraphUpdate();
            ImGui::TreePop();
        }
        return;
    }
	if (ImGui::TreeNode(object->GetName().c_str()))
	{
		for (int index = 0; index < transform->GetChildCount(); index++)
			CreateSceneGraphNode(transform->GetChild(index)->GetObject());
		ImGui::TreePop();
	}
}

std::string Input::GetFullNameHash(Component* component, const std::string& name)
{
    return name + "##" + component->GetObject()->GetName() + "_" + component->GetName();
}
