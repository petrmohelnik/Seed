#pragma once

class Mesh;
class Material;

class FileSystem
{
public:
    void LoadScene(const std::string& path);
    std::vector<std::shared_ptr<Mesh>> LoadMeshes();
    void LoadCameras();
    void UnloadScene();
    std::shared_ptr<Mesh> LoadMesh(const std::string& path);
	std::shared_ptr<Material> LoadMaterial(const std::string& path);
    std::shared_ptr<Mesh> CreateSphere();
    std::shared_ptr<Mesh> CreatePlane();

private:
    const aiScene* GetScene(const std::string& path);
    std::shared_ptr<Mesh> LoadMeshData(aiMesh* assimpMesh);

    std::unordered_map<std::string, std::weak_ptr<Mesh>> meshes;
	std::unordered_map<std::string, std::weak_ptr<Material>> materials;
    Assimp::Importer importer;

    std::string folder = "assets/";
    std::string loadedScene;
};
