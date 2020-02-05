#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

class FileSystem
{
public:
    FileSystem();

    std::string LoadTextFile(const std::string& path);
    Object* LoadObjects(const std::string& path, const std::string& rootName, bool deleteMeshAfterLoad = true);
    std::shared_ptr<Mesh> LoadMesh(const std::string& path, bool deleteAfterLoad = true);
    std::vector<std::shared_ptr<Material>> LoadMaterials(const std::string& path);
    std::unique_ptr<TextureCubeMap> LoadCubeMap(const std::string& path, const std::string& format);
    std::unique_ptr<TextureCubeMap> LoadCubeMapHDR(const std::string& path);
    std::shared_ptr<Texture> LoadTexture(const std::string& path, int bitsMinimum = 0, int bitsMaximum = 0, bool flipHorizontal = false);

protected:
    friend class MeshRenderer;

    void UnloadScene();

private:
    void LoadScene(const std::string& path);
    const aiScene* GetScene(const std::string& path);
    Object* LoadNode(const aiScene* scene, aiNode* node, Object* parent, 
        const std::vector<std::shared_ptr<SubMesh>>& subMeshes, const std::vector<std::shared_ptr<Material>>& materials);
    void LoadMesh(const aiScene* scene, aiNode* node, Object* object, const std::vector<std::shared_ptr<SubMesh>>& subMeshes, const std::vector<std::shared_ptr<Material>>& materials);
    void LoadLight(const aiScene* scene, aiNode* node, Object* object);
    void LoadCamera(const aiScene* scene, Object* object);
    std::shared_ptr<Mesh> LoadMeshData(aiMesh** assimpMeshes, unsigned int numMeshes, bool deleteAfterLoad = true);
    std::shared_ptr<SubMesh> LoadSubMeshData(aiMesh* assimpMesh);
    std::vector<std::shared_ptr<Material>> LoadMaterialsData(aiMaterial** assimpMaterials, unsigned int numMaterials, const std::string& path);
    Material LoadMaterialData(aiMaterial* assimpMaterial, const std::string& folder);
    bool LoadMaterialTexture(aiMaterial* assimpMaterial, aiTextureType textureType, std::shared_ptr<Texture>& textureData, const std::string& folder, int bitsMinimum = 0, int bitsMaximum = 0);
    void LoadMaterialColor(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, std::shared_ptr<Texture>& textureData, aiColor4D defaultColor);
    void LoadMaterialAlpha(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, std::shared_ptr<Texture>& textureData, float defaultAlpha);
    float GetMaterialFloat(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, float defaultAlpha);

    static glm::vec3 ToGlmVec3(const aiVector3D& aiVector);

    std::unordered_map<std::string, std::weak_ptr<Mesh>> loadedMeshes;
    std::unordered_map<std::string, std::vector<std::weak_ptr<Material>>> loadedMaterials;
    std::unordered_map<std::string, std::weak_ptr<Texture>> loadedTextures;
    
    Assimp::Importer importer;

    std::string parentFolder = "assets/";
    std::string loadedScene;
};
