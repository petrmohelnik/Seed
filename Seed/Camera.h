#pragma once
#include "Component.h"
#include "AABB.h"

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

    void UpdateFrustum();
    bool IsInsideFrustum(AABB const& aabb) const;
    void SetProjectionMatrix(float fieldOfView, float aspectRatio, float clipPlaneNear, float clipPlaneFar);

private:
	CameraBlock dataBlock;

    void UpdateFrustumPlanes(glm::mat4 const& viewProjection);
    void UpdateFrustumPoints(glm::mat4 const& viewProjection);
    std::array<glm::vec4, 6> frustumPlanes;
    std::array<glm::vec4, 8> frustumPoints;
};
