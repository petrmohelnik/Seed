#pragma once

class Shader;

class SimpleMesh
{
public:
    enum class Shape
    {
        Quad,
        Cube
    };

    SimpleMesh(Shape shape);
    ~SimpleMesh();
    SimpleMesh(SimpleMesh&& m) = default;
    SimpleMesh(const SimpleMesh& m) = default;
    SimpleMesh& operator=(const SimpleMesh& m) = default;
    SimpleMesh& operator=(SimpleMesh&& m) = default;

    void Draw(Shader* shader);

private:
    struct MeshData
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::uvec3> indices;
    };

    MeshData GenerateQuadData();
    MeshData GenerateCubeData();

    GLuint vbo[2];
    GLuint vao;
    int indicesCount;
};

