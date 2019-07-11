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

    void BindSubMesh(int index);

    int NumberOfSubmeshes();
    int SubmeshIndicesCount(int index);

protected:
    friend class FileSystem;
    friend class MeshRenderer;
    
private:
    struct SubMesh
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec3> bitangents;
        std::vector<glm::vec2> texCoords;
        std::vector<glm::uvec3> indices;
        int indicesCount;
        GLuint vao = 0;
        GLuint vbo[6];

        ~SubMesh();
        void Unload();
    };

    void LoadSubMesh(SubMesh* subMesh);

    bool deleteAfterLoad = true;
    
    std::vector<std::shared_ptr<SubMesh>> subMeshes;
};
