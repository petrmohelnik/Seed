#pragma once
#include "Component.h"
#include "Frustum.h"

class Camera final : public Component
{
public:
    Camera(Object* object);

    struct CameraBlock
    {
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        alignas(16) glm::vec3 viewPos;
    };

    void SetProjectionMatrix(float fieldOfView, float aspectRatio, float clipPlaneNear, float clipPlaneFar);
    float GetFieldOfView();
    float GetNearPlane();
    float GetFarPlane();

    glm::vec3 ScreenPositionToWorld(glm::vec3 position);
    
protected:
    friend class RenderingPipeline;
    friend class Light;
    friend class RenderQueue;

    void BindCamera();
    
    void UpdateFrustum();
    Frustum const& GetFrustum() const;

private:
    CameraBlock dataBlock;

    Frustum frustum;
    float fieldOfView = glm::radians(45.0f);
    float nearPlane = 0.1f;
    float farPlane = 1000.0f;
};
