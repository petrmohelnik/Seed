#pragma once
class Object;
class Renderer;

class Scene
{
public:
	Scene();

	void OnFrameUpdate();
	void Render();

private:
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<std::weak_ptr<Renderer>> renderers;
};
