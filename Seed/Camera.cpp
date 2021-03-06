#include "Camera.h"
#include "RenderingPipeline.h"
#include "Transform.h"
#include "Engine.h"

Camera::Camera(Object* object) : Component(object)
{
    auto windowSize = Engine::GetWindow().GetWindowSize();
    SetProjectionMatrix(fieldOfView, windowSize.x / static_cast<float>(windowSize.y), nearPlane, farPlane);
}

Camera* Camera::Clone()
{
    return new Camera(*this);
}

void Camera::BindCamera()
{
    RenderingPipeline::BindCameraUniform();

    dataBlock.projection = glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
    dataBlock.view = glm::inverse(GetTransform()->GetModelMatrix());
    dataBlock.viewPos = GetTransform()->GetPosition();
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);
}

void Camera::UpdateFrustum()
{
    auto viewProjection = dataBlock.projection * dataBlock.view;
    frustum.Update(viewProjection);
}

void Camera::SetProjectionMatrix(float fieldOfView_, float aspectRatio_, float clipPlaneNear, float clipPlaneFar)
{
    fieldOfView = fieldOfView_;
    aspectRatio = aspectRatio_;
    nearPlane = clipPlaneNear;
    farPlane = clipPlaneFar;
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

glm::vec3 Camera::RayDirectionFromScreenPosition(glm::vec2 screenPosition)
{
    glm::vec3 fromPosition;
    return RayDirectionFromScreenPosition(screenPosition, fromPosition);
}

glm::vec3 Camera::RayDirectionFromScreenPosition(glm::vec2 screenPosition, glm::vec3& fromPosition)
{
    fromPosition = ScreenPositionToWorld(glm::vec3(screenPosition, GetNearPlane()));
    auto toPosition = ScreenPositionToWorld(glm::vec3(screenPosition, GetFarPlane()));
    
    return glm::normalize(toPosition - fromPosition);
}

Frustum const& Camera::GetFrustum() const
{
    return frustum;
}

void Camera::OnInputGraphUpdate()
{
    Engine::GetInput().Text("Camera:");
    Engine::GetInput().SliderFloat("FieldOfView", fieldOfView, 0.5f, 3.14f);
}
