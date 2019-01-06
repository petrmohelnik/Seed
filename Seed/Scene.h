#pragma once

class Objects;
class Components;

class Scene
{
public:
    Scene(Objects& objects,
          Components& components);

	void OnFrameUpdate();
	void Render();
	void CleanObjects();

private:
    Objects& objects;
    Components& components;
};
