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

    struct Frustum
    {
        std::array<glm::vec4, 6> planes;
        std::array<glm::vec4, 8> points;
    };

    void BindCamera();

    void UpdateFrustum();
    void SetProjectionMatrix(float fieldOfView, float aspectRatio, float clipPlaneNear, float clipPlaneFar);

    Frustum const& GetFrustum();

protected:
    friend class RenderingPipeline;

private:
	CameraBlock dataBlock;

    void UpdateFrustumPlanes(glm::mat4 const& viewProjection);
    void UpdateFrustumPoints(glm::mat4 const& viewProjection);

    Frustum frustum;
};
