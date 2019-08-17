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

bool Camera::IsInsideFrustum(AABB const& aabb) const
{
    for (int i = 0; i < 6; i++)
    {
        int out = 0;
        out += ((glm::dot(frustumPlanes[i], glm::vec4(aabb.min.x, aabb.min.y, aabb.min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustumPlanes[i], glm::vec4(aabb.max.x, aabb.min.y, aabb.min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustumPlanes[i], glm::vec4(aabb.min.x, aabb.max.y, aabb.min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustumPlanes[i], glm::vec4(aabb.max.x, aabb.max.y, aabb.min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustumPlanes[i], glm::vec4(aabb.min.x, aabb.min.y, aabb.max.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustumPlanes[i], glm::vec4(aabb.max.x, aabb.min.y, aabb.max.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustumPlanes[i], glm::vec4(aabb.min.x, aabb.max.y, aabb.max.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustumPlanes[i], glm::vec4(aabb.max.x, aabb.max.y, aabb.max.z, 1.0f)) < 0.0) ? 1 : 0);
        if (out == 8) return false;
    }

    int out;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustumPoints[i].x > aabb.max.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustumPoints[i].x < aabb.min.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustumPoints[i].y > aabb.max.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustumPoints[i].y < aabb.min.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustumPoints[i].z > aabb.max.z) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustumPoints[i].z < aabb.min.z) ? 1 : 0); if (out == 8) return false;

    return true;
}

void Camera::SetProjectionMatrix(float fieldOfView, float aspectRatio, float clipPlaneNear, float clipPlaneFar)
{
    dataBlock.projection = glm::perspective(fieldOfView, aspectRatio, clipPlaneNear, clipPlaneFar);
}

void Camera::UpdateFrustumPlanes(glm::mat4 const& viewProjection)
{
    frustumPlanes[0].x = viewProjection[0][3] + viewProjection[0][0];
    frustumPlanes[0].y = viewProjection[1][3] + viewProjection[1][0];
    frustumPlanes[0].z = viewProjection[2][3] + viewProjection[2][0];
    frustumPlanes[0].w = viewProjection[3][3] + viewProjection[3][0];
    frustumPlanes[0] /= glm::length(glm::vec3(frustumPlanes[0]));

    frustumPlanes[1].x = viewProjection[0][3] - viewProjection[0][0];
    frustumPlanes[1].y = viewProjection[1][3] - viewProjection[1][0];
    frustumPlanes[1].z = viewProjection[2][3] - viewProjection[2][0];
    frustumPlanes[1].w = viewProjection[3][3] - viewProjection[3][0];
    frustumPlanes[1] /= glm::length(glm::vec3(frustumPlanes[1]));

    frustumPlanes[2].x = viewProjection[0][3] - viewProjection[0][1];
    frustumPlanes[2].y = viewProjection[1][3] - viewProjection[1][1];
    frustumPlanes[2].z = viewProjection[2][3] - viewProjection[2][1];
    frustumPlanes[2].w = viewProjection[3][3] - viewProjection[3][1];
    frustumPlanes[2] /= glm::length(glm::vec3(frustumPlanes[2]));

    frustumPlanes[3].x = viewProjection[0][3] + viewProjection[0][1];
    frustumPlanes[3].y = viewProjection[1][3] + viewProjection[1][1];
    frustumPlanes[3].z = viewProjection[2][3] + viewProjection[2][1];
    frustumPlanes[3].w = viewProjection[3][3] + viewProjection[3][1];
    frustumPlanes[3] /= glm::length(glm::vec3(frustumPlanes[3]));

    frustumPlanes[4].x = viewProjection[0][2];
    frustumPlanes[4].y = viewProjection[1][2];
    frustumPlanes[4].z = viewProjection[2][2];
    frustumPlanes[4].w = viewProjection[3][2];
    frustumPlanes[4] /= glm::length(glm::vec3(frustumPlanes[4]));

    frustumPlanes[5].x = viewProjection[0][3] - viewProjection[0][2];
    frustumPlanes[5].y = viewProjection[1][3] - viewProjection[1][2];
    frustumPlanes[5].z = viewProjection[2][3] - viewProjection[2][2];
    frustumPlanes[5].w = viewProjection[3][3] - viewProjection[3][2];
    frustumPlanes[5] /= glm::length(glm::vec3(frustumPlanes[5]));
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
        frustumPoints[i] = clipSpaceToWorld * cube[i];
        frustumPoints[i] /= frustumPoints[i].w;
    }
}
