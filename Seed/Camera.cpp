#include "Camera.h"
#include "RenderingPipeline.h"
#include "Transform.h"
#include "Engine.h"

Camera::Camera(Object* object) : Component(object)
{
    auto windowSize = Engine::GetWindow().GetWindowSize();
    SetProjectionMatrix(fieldOfView, windowSize.x / static_cast<float>(windowSize.y), nearPlane, farPlane);
}

void Camera::BindCamera()
{
    RenderingPipeline::BindCameraUniform();

    dataBlock.view = glm::inverse(GetTransform()->GetModelMatrix());
    dataBlock.viewPos = GetTransform()->GetPosition();
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);
}

void Camera::UpdateFrustum()
{
    auto viewProjection = dataBlock.projection * dataBlock.view;
    frustum.Update(viewProjection);
}

void Camera::SetProjectionMatrix(float fieldOfView_, float aspectRatio, float clipPlaneNear, float clipPlaneFar)
{
    fieldOfView = fieldOfView_;
    nearPlane = clipPlaneNear;
    farPlane = clipPlaneFar;
    dataBlock.projection = glm::perspective(fieldOfView, aspectRatio, clipPlaneNear, clipPlaneFar);
}

float Camera::GetFieldOfView()
{
    return fieldOfView;
}

float Camera::GetNearPlane()
{
    return nearPlane;
}

float Camera::GetFarPlane()
{
    return farPlane;
}

glm::vec3 Camera::ScreenPositionToWorld(glm::vec3 position)
{
    auto windowSize = Engine::GetWindow().GetWindowSize();
    position.y = windowSize.y - 1.0f - position.y;

    auto worldDistance = glm::clamp(position.z, nearPlane, farPlane);
    position.z = (worldDistance - nearPlane) / (farPlane - nearPlane);

    return glm::unProject(position, dataBlock.view, dataBlock.projection, glm::vec4(0.0f, 0.0f, windowSize));
}

Frustum const& Camera::GetFrustum() const
{
    return frustum;
}
