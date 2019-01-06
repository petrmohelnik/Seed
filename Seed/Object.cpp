#include "Object.h"
#include "Transform.h"
#include "Renderer.h"
#include "Script.h"

Object::Object(Components& components) 
    : components(components)
{
	transform = std::make_shared<Transform>();
}

void Object::SetRenderer(std::shared_ptr<Renderer> renderer_)
{
	renderer = renderer_;
	renderer->SetObject(weak_from_this());
}

void Object::AddScript(std::shared_ptr<Script> script)
{
	scripts.push_back(script);
	script->SetObject(weak_from_this());
}
