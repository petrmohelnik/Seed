#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

class FileSystem
{
public:
    FileSystem();

    std::string LoadTextFile(const std::string& path);
    void LoadScene(const std::string& path);
    std::vector<std::shared_ptr<Mesh>> LoadMeshes(); //we need to somehow return transform, can we return it separately?
    std::vector<std::vector<std::shared_ptr<Material>>> LoadMaterials();
    std::vector<Object*> LoadCameras(); //return components? it will contain position an such
    void LoadLights();
    void UnloadScene();
    std::shared_ptr<Mesh> LoadMesh(const std::string& path);
    std::vector<std::shared_ptr<Material>> LoadMaterials(const std::string& path);
    std::shared_ptr<TextureCubeMap> LoadCubeMap(const std::string& path, const std::string& format);

private:
    const aiScene* GetScene(const std::string& path);
    std::shared_ptr<Mesh> LoadMeshData(aiMesh** assimpMeshes, unsigned int numMeshes);
    Mesh::SubMesh LoadSubMeshData(aiMesh* assimpMesh);
    std::vector<std::shared_ptr<Material>> LoadMaterialsData(aiMaterial** assimpMaterials, unsigned int numMaterials, const std::string& folder);
    Material LoadMaterialData(aiMaterial* assimpMaterial, const std::string& folder);
    bool LoadMaterialTexture(aiMaterial* assimpMaterial, aiTextureType textureType, std::shared_ptr<Texture>& textureData, const std::string& folder, int bits = 0);
    void LoadMaterialColor(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, std::shared_ptr<Texture>& textureData, aiColor4D defaultColor);
    void LoadMaterialAlpha(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, std::shared_ptr<Texture>& textureData, float defaultAlpha);
    float GetMaterialFloat(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, float defaultAlpha);
    std::shared_ptr<Texture> LoadTexture(const std::string& path, int bits = 0, bool flipHorizontal = false);

    std::unordered_map<std::string, std::weak_ptr<Mesh>> loadedMeshes;
	std::unordered_map<std::string, std::vector<std::weak_ptr<Material>>> loadedMaterials;
	std::unordered_map<std::string, std::weak_ptr<TextureCubeMap>> loadedCubeMaps;
    
	Assimp::Importer importer;

    std::string parentFolder = "assets/";
    std::string loadedScene;
};
