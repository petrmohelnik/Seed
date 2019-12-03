#include "Frustum.h"

void Frustum::Update(glm::mat4 const& viewProjection)
{
    UpdatePlanes(viewProjection);
    UpdatePoints(viewProjection);
}

glm::vec4 const& Frustum::GetPlane(int index) const
{
    return planes[index];
}

glm::vec4 const& Frustum::GetPoint(int index) const
{
    return points[index];
}

void Frustum::UpdatePlanes(glm::mat4 const& viewProjection)
{
    planes[0].x = viewProjection[0][3] + viewProjection[0][0];
    planes[0].y = viewProjection[1][3] + viewProjection[1][0];
    planes[0].z = viewProjection[2][3] + viewProjection[2][0];
    planes[0].w = viewProjection[3][3] + viewProjection[3][0];
    planes[0] /= glm::length(glm::vec3(planes[0]));

    planes[1].x = viewProjection[0][3] - viewProjection[0][0];
    planes[1].y = viewProjection[1][3] - viewProjection[1][0];
    planes[1].z = viewProjection[2][3] - viewProjection[2][0];
    planes[1].w = viewProjection[3][3] - viewProjection[3][0];
    planes[1] /= glm::length(glm::vec3(planes[1]));

    planes[2].x = viewProjection[0][3] - viewProjection[0][1];
    planes[2].y = viewProjection[1][3] - viewProjection[1][1];
    planes[2].z = viewProjection[2][3] - viewProjection[2][1];
    planes[2].w = viewProjection[3][3] - viewProjection[3][1];
    planes[2] /= glm::length(glm::vec3(planes[2]));

    planes[3].x = viewProjection[0][3] + viewProjection[0][1];
    planes[3].y = viewProjection[1][3] + viewProjection[1][1];
    planes[3].z = viewProjection[2][3] + viewProjection[2][1];
    planes[3].w = viewProjection[3][3] + viewProjection[3][1];
    planes[3] /= glm::length(glm::vec3(planes[3]));

    planes[4].x = viewProjection[0][2];
    planes[4].y = viewProjection[1][2];
    planes[4].z = viewProjection[2][2];
    planes[4].w = viewProjection[3][2];
    planes[4] /= glm::length(glm::vec3(planes[4]));

    planes[5].x = viewProjection[0][3] - viewProjection[0][2];
    planes[5].y = viewProjection[1][3] - viewProjection[1][2];
    planes[5].z = viewProjection[2][3] - viewProjection[2][2];
    planes[5].w = viewProjection[3][3] - viewProjection[3][2];
    planes[5] /= glm::length(glm::vec3(planes[5]));
}

void Frustum::UpdatePoints(glm::mat4 const& viewProjection)
{
    static std::array<glm::vec4, 8> cube =
    {
        glm::vec4(-1, -1, 0, 1),
        glm::vec4(-1,  1, 0, 1),
        glm::vec4(1,  1, 0, 1),
        glm::vec4(1, -1, 0, 1),
        glm::vec4(-1, -1, 1, 1),
        glm::vec4(-1,  1, 1, 1),
        glm::vec4(1,  1, 1, 1),
        glm::vec4(1, -1, 1, 1)
    };

    auto clipSpaceToWorld = glm::inverse(viewProjection);
    for (int i = 0; i < 8; i++)
    {
        points[i] = clipSpaceToWorld * cube[i];
        points[i] /= points[i].w;
    }
}
