#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"
//#include "Engine.h"

class FileSystem
{
public:
    FileSystem();

    void LoadScene(const std::string& path);
    std::vector<std::shared_ptr<Mesh>> LoadMeshes(); //we need to somehow return position
    std::vector<std::shared_ptr<Material>> LoadMaterials();
    std::vector<Object*> LoadCameras(); //return components? it will contain position an such
    void LoadLights();
    void UnloadScene();
    std::shared_ptr<Mesh> LoadMesh(const std::string& path);
    std::vector<std::shared_ptr<Material>> LoadMaterials(const std::string& path);
    std::shared_ptr<Mesh> CreateSphere();
    std::shared_ptr<Mesh> CreatePlane();

private:
    const aiScene* GetScene(const std::string& path);
    std::shared_ptr<Mesh> LoadMeshData(aiMesh** assimpMeshes, unsigned int numMeshes);
    Mesh::SubMesh LoadSubMeshData(aiMesh* assimpMesh);
    std::vector<std::shared_ptr<Material>> LoadMaterialsData(aiMaterial** assimpMaterials, unsigned int numMaterials);
    Material LoadMaterialData(aiMaterial* assimpMaterial);
    Texture LoadTexture(const std::string& path);

    std::unordered_map<std::string, std::weak_ptr<Mesh>> loadedMeshes;
	std::unordered_map<std::string, std::vector<std::weak_ptr<Material>>> loadedMaterials;
    Assimp::Importer importer;

    std::string folder = "assets/";
    std::string loadedScene;
};
