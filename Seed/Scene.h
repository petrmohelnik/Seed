#pragma once
class Object;
class Renderer;
class Script;

class Scene
{
public:
	Scene();

	void OnFrameUpdate();
	void Render();
	void CleanObjects();

	void AddObject(std::shared_ptr<Object> object);
	void RegisterRenderer(std::weak_ptr<Renderer> renderer);
	void RegisterScript(std::weak_ptr<Script> script);

private:
	static std::vector<std::shared_ptr<Object>> objects;
	static std::vector<std::weak_ptr<Renderer>> renderers;
	static std::vector<std::weak_ptr<Script>> scripts;
};
