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

void Camera::UpdateFrustum()
{
    auto viewProjection = dataBlock.projection * dataBlock.view;
    UpdateFrustumPlanes(viewProjection);
    UpdateFrustumPoints(viewProjection);
}

void Camera::SetProjectionMatrix(float fieldOfView, float aspectRatio, float clipPlaneNear, float clipPlaneFar)
{
    dataBlock.projection = glm::perspective(fieldOfView, aspectRatio, clipPlaneNear, clipPlaneFar);
}

Camera::Frustum const& Camera::GetFrustum()
{
    return frustum;
}

void Camera::UpdateFrustumPlanes(glm::mat4 const& viewProjection)
{
    frustum.planes[0].x = viewProjection[0][3] + viewProjection[0][0];
    frustum.planes[0].y = viewProjection[1][3] + viewProjection[1][0];
    frustum.planes[0].z = viewProjection[2][3] + viewProjection[2][0];
    frustum.planes[0].w = viewProjection[3][3] + viewProjection[3][0];
    frustum.planes[0] /= glm::length(glm::vec3(frustum.planes[0]));

    frustum.planes[1].x = viewProjection[0][3] - viewProjection[0][0];
    frustum.planes[1].y = viewProjection[1][3] - viewProjection[1][0];
    frustum.planes[1].z = viewProjection[2][3] - viewProjection[2][0];
    frustum.planes[1].w = viewProjection[3][3] - viewProjection[3][0];
    frustum.planes[1] /= glm::length(glm::vec3(frustum.planes[1]));

    frustum.planes[2].x = viewProjection[0][3] - viewProjection[0][1];
    frustum.planes[2].y = viewProjection[1][3] - viewProjection[1][1];
    frustum.planes[2].z = viewProjection[2][3] - viewProjection[2][1];
    frustum.planes[2].w = viewProjection[3][3] - viewProjection[3][1];
    frustum.planes[2] /= glm::length(glm::vec3(frustum.planes[2]));

    frustum.planes[3].x = viewProjection[0][3] + viewProjection[0][1];
    frustum.planes[3].y = viewProjection[1][3] + viewProjection[1][1];
    frustum.planes[3].z = viewProjection[2][3] + viewProjection[2][1];
    frustum.planes[3].w = viewProjection[3][3] + viewProjection[3][1];
    frustum.planes[3] /= glm::length(glm::vec3(frustum.planes[3]));

    frustum.planes[4].x = viewProjection[0][2];
    frustum.planes[4].y = viewProjection[1][2];
    frustum.planes[4].z = viewProjection[2][2];
    frustum.planes[4].w = viewProjection[3][2];
    frustum.planes[4] /= glm::length(glm::vec3(frustum.planes[4]));

    frustum.planes[5].x = viewProjection[0][3] - viewProjection[0][2];
    frustum.planes[5].y = viewProjection[1][3] - viewProjection[1][2];
    frustum.planes[5].z = viewProjection[2][3] - viewProjection[2][2];
    frustum.planes[5].w = viewProjection[3][3] - viewProjection[3][2];
    frustum.planes[5] /= glm::length(glm::vec3(frustum.planes[5]));
}

void Camera::UpdateFrustumPoints(glm::mat4 const& viewProjection)
{
    static std::array<glm::vec4, 8> cube =
    {
        glm::vec4(-1, -1, 0, 1),
        glm::vec4(-1,  1, 0, 1),
        glm::vec4( 1,  1, 0, 1),
        glm::vec4( 1, -1, 0, 1),
        glm::vec4(-1, -1, 1, 1),
        glm::vec4(-1,  1, 1, 1),
        glm::vec4( 1,  1, 1, 1),
        glm::vec4( 1, -1, 1, 1)
    };

    auto clipSpaceToWorld = glm::inverse(viewProjection);
    for (int i = 0; i < 8; i++)
    {
        frustum.points[i] = clipSpaceToWorld * cube[i];
        frustum.points[i] /= frustum.points[i].w;
    }
}
