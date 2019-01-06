#pragma once

class Objects;
class Components;

class SceneDefinition
{
public:
    SceneDefinition(Objects& objects,
                    Components& components);

    void InitializeScene(std::string scene);

private:
    Objects& objects;
    Components& components;
};
