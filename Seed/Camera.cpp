#include "Camera.h"
#include "RenderingPipeline.h"
#include "Transform.h"

void Camera::BindCamera()
{
    RenderingPipeline::BindCameraUniform();

	dataBlock.view = glm::inverse(GetTransform()->GetModelMatrix());
	dataBlock.viewPos = GetTransform()->GetPosition();
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);
}

void Camera::SetProjectionMatrix(float fieldOfView, float aspectRatio, float clipPlaneNear, float clipPlaneFar)
{
    dataBlock.projection = glm::perspective(fieldOfView, aspectRatio, clipPlaneNear, clipPlaneFar);
}
