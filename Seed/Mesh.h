#pragma once

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void Load();
    void Unload();

protected:
    friend class FileSystem;
    
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<std::vector<glm::uvec3>> indices;

private:
    GLuint vao;
};
