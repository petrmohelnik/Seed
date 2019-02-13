#pragma once

class Mesh
{
public:
    Mesh();
    ~Mesh();
    Mesh(Mesh&& m) = default;
    Mesh(const Mesh& m) = default;
    Mesh& operator=(const Mesh& m) = default;
    Mesh& operator=(Mesh&& m) = default;

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
        GLuint vbo[4];
    };

    void LoadSubMesh(SubMesh& subMesh);

    bool deleteAfterLoad = true;
    
    std::vector<SubMesh> subMeshes;
};
