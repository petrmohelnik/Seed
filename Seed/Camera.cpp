#include "Camera.h"
#include "RenderingPipeline.h"

void Camera::BindCamera()
{
    RenderingPipeline::BindCameraUniform();
    auto view = glm::mat4(1.0);
    CameraBlock cameraBlockData
	{
		glm::perspective(glm::radians(45.0f), 16 / 9.0f, 0.1f, 1000.0f),
        glm::translate(view, glm::vec3(0.0, 0.0, -10.0)),
        glm::vec4(0.0, 0.0, 1000.0, 0.0)
    };
    glBufferData(GL_UNIFORM_BUFFER, sizeof(cameraBlockData), &cameraBlockData, GL_DYNAMIC_DRAW);
}
