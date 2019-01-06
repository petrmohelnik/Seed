#include "Object.h"
#include "Script.h"
#include "Renderer.h"

Object::Object()
{
}

void Object::Start()
{
	for (const auto& script : scripts)
	{
		script->Start();
	}
}

void Object::OnFrameUpdate()
{
	for (const auto& script : scripts)
	{
		script->OnFrameUpdate();
	}
}

void Object::End()
{
	for (const auto& script : scripts)
	{
		script->End();
	}
}

void Object::SetRenderer(std::shared_ptr<Renderer> renderer_)
{
	renderer = renderer_;
	renderer->SetObject(weak_from_this());
}
