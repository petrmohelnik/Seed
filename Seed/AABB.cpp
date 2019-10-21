#include "AABB.h"

AABB::AABB(glm::vec3 min, glm::vec3 max) : min(min), max(max)
{
}

AABB AABB::GetTransformedAABB(glm::mat4 const& transformation) const
{
    auto vertices = GetBoxVertices();
    for (auto& vertex : vertices)
        vertex = transformation * glm::vec4(vertex, 1.0f);

    AABB transformedAABB;
    for (auto const& vertex : vertices)
    {
        if (vertex.x < transformedAABB.min.x)
            transformedAABB.min.x = vertex.x;
        if (vertex.y < transformedAABB.min.y)
            transformedAABB.min.y = vertex.y;
        if (vertex.z < transformedAABB.min.z)
            transformedAABB.min.z = vertex.z;

        if (vertex.x > transformedAABB.max.x)
            transformedAABB.max.x = vertex.x;
        if (vertex.y > transformedAABB.max.y)
            transformedAABB.max.y = vertex.y;
        if (vertex.z > transformedAABB.max.z)
            transformedAABB.max.z = vertex.z;
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

bool AABB::TestFrustum(const Camera::Frustum& frustum) const
{
    for (int i = 0; i < 6; i++)
    {
        int out = 0;
        out += ((glm::dot(frustum.planes[i], glm::vec4(min.x, min.y, min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.planes[i], glm::vec4(max.x, min.y, min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.planes[i], glm::vec4(min.x, max.y, min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.planes[i], glm::vec4(max.x, max.y, min.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.planes[i], glm::vec4(min.x, min.y, max.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.planes[i], glm::vec4(max.x, min.y, max.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.planes[i], glm::vec4(min.x, max.y, max.z, 1.0f)) < 0.0) ? 1 : 0);
        out += ((glm::dot(frustum.planes[i], glm::vec4(max.x, max.y, max.z, 1.0f)) < 0.0) ? 1 : 0);
        if (out == 8) return false;
    }

    int out;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.points[i].x > max.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.points[i].x < min.x) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.points[i].y > max.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.points[i].y < min.y) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.points[i].z > max.z) ? 1 : 0); if (out == 8) return false;
    out = 0; for (int i = 0; i < 8; i++) out += ((frustum.points[i].z < min.z) ? 1 : 0); if (out == 8) return false;

    return true;
}
