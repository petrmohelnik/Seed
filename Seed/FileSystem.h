#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

class FileSystem
{
public:
    FileSystem();

    std::string LoadTextFile(const std::string& path);
    std::vector<Object*> LoadObjects(const std::string& path);
    std::shared_ptr<Mesh> LoadMesh(const std::string& path);
    std::vector<std::shared_ptr<Material>> LoadMaterials(const std::string& path);
    std::unique_ptr<TextureCubeMap> LoadCubeMap(const std::string& path, const std::string& format);
    std::unique_ptr<TextureCubeMap> LoadCubeMapHDR(const std::string& path);

private:
    void LoadScene(const std::string& path);
    void UnloadScene();
    const aiScene* GetScene(const std::string& path);
    void LoadNode(const aiScene* scene, aiNode* node, Object* parent, std::vector<Object*>& objects, const std::vector<std::shared_ptr<Material>>& materials);
    void LoadMesh(const aiScene* scene, aiNode* node, Object* object, const std::vector<std::shared_ptr<Material>>& materials);
    void LoadLight(const aiScene* scene, Object* object);
    void LoadCamera(const aiScene* scene, Object* object);
    std::shared_ptr<Mesh> LoadMeshData(aiMesh** assimpMeshes, unsigned int numMeshes, unsigned* assimpMeshesIndexes = nullptr);
    Mesh::SubMesh LoadSubMeshData(aiMesh* assimpMesh);
    std::vector<std::shared_ptr<Material>> LoadMaterialsData(aiMaterial** assimpMaterials, unsigned int numMaterials, const std::string& path);
    Material LoadMaterialData(aiMaterial* assimpMaterial, const std::string& folder);
    bool LoadMaterialTexture(aiMaterial* assimpMaterial, aiTextureType textureType, std::shared_ptr<Texture>& textureData, const std::string& folder, int bits = 0);
    void LoadMaterialColor(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, std::shared_ptr<Texture>& textureData, aiColor4D defaultColor);
    void LoadMaterialAlpha(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, std::shared_ptr<Texture>& textureData, float defaultAlpha);
    float GetMaterialFloat(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, float defaultAlpha);
    std::shared_ptr<Texture> LoadTexture(const std::string& path, int bits = 0, bool flipHorizontal = false);

    std::unordered_map<std::string, std::weak_ptr<Mesh>> loadedMeshes;
	std::unordered_map<std::string, std::vector<std::weak_ptr<Material>>> loadedMaterials;
    
	Assimp::Importer importer;

    std::string parentFolder = "assets/";
    std::string loadedScene;
};
