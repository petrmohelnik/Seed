#include "Camera.h"
#include "RenderingPipeline.h"
#include "Transform.h"

void Camera::BindCamera()
{
    RenderingPipeline::BindCameraUniform();

    CameraBlock cameraBlockData
	{
		glm::perspective(glm::radians(45.0f), 16 / 9.0f, 0.1f, 1000.0f),
        glm::inverse(GetTransform()->GetModelMatrix()),
        GetTransform()->GetPosition()
    };
    glBufferData(GL_UNIFORM_BUFFER, sizeof(cameraBlockData), &cameraBlockData, GL_DYNAMIC_DRAW);
}
