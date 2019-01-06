#include "Scene.h"
#include "Script.h"
#include "Object.h"
#include "Renderer.h"

std::vector<std::shared_ptr<Object>> Scene::objects;
std::vector<std::weak_ptr<Renderer>> Scene::renderers;
std::vector<std::weak_ptr<Script>> Scene::scripts;

Scene::Scene()
{
}

void Scene::OnFrameUpdate()
{
	for (const auto& script : scripts)
	{
		script.lock()->OnFrameUpdate();
	}
}

void Scene::Render()
{
	for (const auto& renderer : renderers)
	{
		renderer.lock()->Render();
	}
}

void Scene::CleanObjects()
{
	std::remove_if(std::begin(scripts), std::end(scripts), [](const auto& script)
	{
		return script.expired();
	});

	std::remove_if(std::begin(renderers), std::end(renderers), [](const auto& renderer)
	{
		return renderer.expired();
	});
}

void Scene::AddObject(std::shared_ptr<Object> object)
{
	objects.push_back(object);
}

void Scene::RegisterRenderer(std::weak_ptr<Renderer> renderer)
{
	renderers.push_back(renderer);
}

void Scene::RegisterScript(std::weak_ptr<Script> script)
{
	scripts.push_back(script);
}
