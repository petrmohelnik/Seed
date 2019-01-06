#pragma once
class Script;
class Renderer;

class Object : public std::enable_shared_from_this<Object>
{
public:
	Object();
	virtual ~Object() = default;

	void Start();
	void OnFrameUpdate();
	void End();

	void SetRenderer(std::shared_ptr<Renderer> renderer_);

private:
	std::shared_ptr<Renderer> renderer;
	std::vector<std::unique_ptr<Script>> scripts;
};
