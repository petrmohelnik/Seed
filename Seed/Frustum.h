#pragma once
class Frustum
{
public:
    void Update(glm::mat4 const& viewProjection);
    glm::vec4 const& GetPlane(int index) const;
    glm::vec4 const& GetPoint(int index) const;
    bool TestSphere(glm::vec3 center, float radius) const;

private:
    void UpdatePlanes(glm::mat4 const& viewProjection);
    void UpdatePoints(glm::mat4 const& viewProjection);

    std::array<glm::vec4, 6> planes;
    std::array<glm::vec4, 8> points;
};

