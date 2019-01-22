#pragma once
#include "Mesh.h"
#include "Material.h"

class FileSystem
{
public:
    FileSystem();

    void LoadScene(const std::string& path);
    std::vector<std::shared_ptr<Mesh>> LoadMeshes();
    void LoadCameras();
    void UnloadScene();
    std::shared_ptr<Mesh> LoadMesh(const std::string& path);
    std::vector<std::shared_ptr<Material>> LoadMaterials(const std::string& path);
    std::shared_ptr<Mesh> CreateSphere();
    std::shared_ptr<Mesh> CreatePlane();

private:
    const aiScene* GetScene(const std::string& path);
    std::shared_ptr<Mesh> LoadMeshData(aiMesh** assimpMeshes, unsigned int numMeshes);
    Mesh::SubMesh LoadSubMeshData(aiMesh* assimpMesh);
    void LoadMaterialsData();
    void LoadMaterialData();

    std::unordered_map<std::string, std::weak_ptr<Mesh>> meshes;
	std::unordered_map<std::string, std::vector<std::weak_ptr<Material>>> materials;
    Assimp::Importer importer;

    std::string folder = "assets/";
    std::string loadedScene;
};
