#include "Scene.h"
#include "Object.h"
#include "Renderer.h"

Scene::Scene()
{
}

void Scene::OnFrameUpdate()
{
	for (const auto& object : objects)
	{
		object->OnFrameUpdate();
	}
}

void Scene::Render()
{
	std::remove_if(std::begin(renderers), std::end(renderers), [] (const auto& renderer)
	{
		if (auto rendererSharedPtr = renderer.lock())
		{
			rendererSharedPtr->Render();
			return false;
		}
		return true;
	});
}
