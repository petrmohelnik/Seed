#include "Camera.h"
#include "RenderingPipeline.h"
#include "Transform.h"

void Camera::BindCamera()
{
    RenderingPipeline::BindCameraUniform();

    auto view = glm::inverse(GetTransform()->GetModelMatrix());
    auto position = glm::vec4(GetTransform()->GetPosition(), 0.0f);
    
    CameraBlock cameraBlockData
	{
		glm::perspective(glm::radians(45.0f), 16 / 9.0f, 0.1f, 1000.0f),
        view,
        position
    };
    glBufferData(GL_UNIFORM_BUFFER, sizeof(cameraBlockData), &cameraBlockData, GL_DYNAMIC_DRAW);
}
