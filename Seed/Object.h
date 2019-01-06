#pragma once

class Components;
class Transform;
class Renderer;
class Script;

class Object : public std::enable_shared_from_this<Object>
{
public:
	Object(Components& components);
	virtual ~Object() = default;

protected:
	void SetRenderer(std::shared_ptr<Renderer> renderer_);
	void AddScript(std::shared_ptr<Script> script);

private:
    Components& components;

	std::shared_ptr<Transform> transform;
	std::shared_ptr<Renderer> renderer;
	std::vector<std::shared_ptr<Script>> scripts;
};
