#pragma once

class Mesh
{
public:
    ~Mesh();

    void Load();
    void Unload();

    static std::shared_ptr<Mesh> LoadFromFile();
    static std::shared_ptr<Mesh> CreateSphere();
    static std::shared_ptr<Mesh> CreatePlane();

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uv;
    std::vector<std::vector<glm::uvec3>> indices;

    GLuint vao;
};
