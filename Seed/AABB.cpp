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
