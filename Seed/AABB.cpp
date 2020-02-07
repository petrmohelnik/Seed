#include "AABB.h"
#include "Frustum.h"

AABB::AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max)
{
}

AABB AABB::GetTransformedAABB(glm::mat4 const& transformation) const
{
    AABB transformedAABB(transformation[3], transformation[3]);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            auto a = transformation[j][i] * min[j];
            auto b = transformation[j][i] * max[j];
            if (a < b)
            {
                transformedAABB.min[i] += a;
                transformedAABB.max[i] += b;
            }
            else
            {
                transformedAABB.min[i] += b;
                transformedAABB.max[i] += a;
            }
        }
    }

    return transformedAABB;
}

std::array<glm::vec3, 8> AABB::GetBoxVertices() const
{
    std::array<glm::vec3, 8> vertices
    {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(max.x, max.y, max.z),
    };

    return vertices;
}

bool AABB::TestSphere(glm::vec3 center, float radius) const
{
    float sqrDist = 0.0f;
    for (int i = 0; i < 3; i++) {
        if (center[i] < min[i]) sqrDist += (min[i] - center[i]) * (min[i] - center[i]);
        if (center[i] > max[i]) sqrDist += (center[i] - max[i]) * (center[i] - max[i]);
    }

    return sqrDist <= radius * radius;
}

bool AABB::TestFrustum(const Frustum& frustum) const
{
    for (int i = 0; i < 6; i++)
    {
        int out = 0;
        out += ((glm::dot(frustum.GetPlane(i), glm::vec4(min.x, min.y, min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.GetPlane(i), glm::vec4(max.x, min.y, min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.GetPlane(i), glm::vec4(min.x, max.y, min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.GetPlane(i), glm::vec4(max.x, max.y, min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.GetPlane(i), glm::vec4(min.x, min.y, max.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.GetPlane(i), glm::vec4(max.x, min.y, max.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.GetPlane(i), glm::vec4(min.x, max.y, max.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.GetPlane(i), glm::vec4(max.x, max.y, max.z, 1.0f)) < 0.0) ? 1 : 0);
        if (out == 8) return false;
    }

    int out;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.GetPoint(i).x > max.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.GetPoint(i).x < min.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.GetPoint(i).y > max.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.GetPoint(i).y < min.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.GetPoint(i).z > max.z) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.GetPoint(i).z < min.z) ? 1 : 0); if (out == 8) return false;

    return true;
}
