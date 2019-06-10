#pragma once
#include "Component.h"

class Camera final : public Component
{
public:
    using Component::Component;
    struct CameraBlock
    {
        glm::mat4 view;
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16 / 9.0f, 0.1f, 1000.0f);
        alignas(16) glm::vec3 viewPos;
    };

    void BindCamera();

private:

	CameraBlock dataBlock;
};
