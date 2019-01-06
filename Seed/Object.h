#pragma once
#include "Components.h"

class Transform;
class Renderer;
class Script;

class Object : public std::enable_shared_from_this<Object>
{
public:
	Object(Components& components);
	virtual ~Object() = default;

    template <class T, typename std::enable_if<std::is_base_of<Script, T>::value>::type* = nullptr>
    T* GetComponent();

protected:
    virtual void Initialize();
	void SetRenderer(std::shared_ptr<Renderer> renderer_);
	void AddScript(std::shared_ptr<Script> script);

    Components& components;

private:
	std::shared_ptr<Transform> transform;
	std::shared_ptr<Renderer> renderer;
	std::vector<std::shared_ptr<Script>> scripts;
};

template<class T, typename std::enable_if<std::is_base_of<Script, T>::value>::type*>
inline T* Object::GetComponent()
{
    T* scriptRawPointer = nullptr;
    for (const auto& script : scripts)
    {
        if ((scriptRawPointer = dynamic_cast<T*>(script.get())) != nullptr)
            return scriptRawPointer;
    }
    return nullptr;
}
