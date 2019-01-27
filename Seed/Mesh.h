#pragma once

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void Load();
    void Unload();

    int NumberOfSubmeshes();

protected:
    friend class FileSystem;
    
private:
    struct SubMesh
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::uvec3> indices;
        GLuint vao;
    };

    bool deleteAfterLoad = false;
    
    std::vector<SubMesh> subMeshes;
};
