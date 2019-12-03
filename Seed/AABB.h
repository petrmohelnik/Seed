#pragma once
#include "Camera.h"

class Frustum;

class AABB
{
public:
    AABB() = default;
    AABB(glm::vec3 min, glm::vec3 max);

    AABB GetTransformedAABB(glm::mat4 const& transformation) const;
    std::array<glm::vec3, 8> GetBoxVertices() const;
    bool TestSphere(glm::vec3 center, float radius) const;
    bool TestFrustum(const Frustum& frustum) const;

    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(std::numeric_limits<float>::lowest());;
};

