#pragma once
#include "Component.h"

class Camera final : public Component
{
public:
    using Component::Component;

    void BindCamera();

private:
    struct CameraBlock
    {
        glm::mat4 projection;
        glm::mat4 view;
        alignas(16) glm::vec3 viewPos;
    };
};
