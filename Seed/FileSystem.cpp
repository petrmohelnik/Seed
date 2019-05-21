#include "FileSystem.h"
#include <FreeImage.h>
#include "Engine.h"

FileSystem::FileSystem()
{
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType::aiPrimitiveType_LINE | aiPrimitiveType::aiPrimitiveType_POINT);
}

std::string FileSystem::LoadTextFile(const std::string& path)
{
    std::ifstream file(path);

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

void FileSystem::LoadScene(const std::string& path)
{
    if (loadedScene == path)
        return;

    auto scene = importer.ReadFile(parentFolder + path,
        aiProcess_GenSmoothNormals | 
        aiProcess_SortByPType | 
        aiProcess_Triangulate |
        aiProcess_CalcTangentSpace |
        aiProcess_FixInfacingNormals
    );

    if (!scene)
        throw std::runtime_error("Failed to parse " + path + " file : " + importer.GetErrorString());
    loadedScene = path;
}

const aiScene* FileSystem::GetScene(const std::string& path)
{
    LoadScene(path);
    return importer.GetScene();
}

std::vector<std::shared_ptr<Mesh>> FileSystem::LoadMeshes()
{
    std::vector<std::shared_ptr<Mesh>> meshes;
    auto scene = importer.GetScene();

    meshes.push_back(LoadMeshData(scene->mMeshes, scene->mNumMeshes)); //temporary, it needs to separate meshes based on geometry, this return only one

    return meshes;
}

std::vector<std::vector<std::shared_ptr<Material>>> FileSystem::LoadMaterials()
{
    //needs to return vector of materials for each mesh
    auto scene = importer.GetScene();
    return std::vector<std::vector<std::shared_ptr<Material>>>();//LoadMaterialsData(scene->mMaterials, scene->mNumMaterials);
}

std::vector<Object*> FileSystem::LoadCameras()
{
    std::vector<Object*> cameras;
    auto object = Engine::GetObjects().CreateObject<Object>("camera1");
    cameras.push_back(object);

    return cameras;
}

void FileSystem::UnloadScene()
{
    importer.FreeScene();
}

std::shared_ptr<Mesh> FileSystem::LoadMesh(const std::string& path)
{
    auto result = loadedMeshes.find(path);
    if (result != loadedMeshes.end() && !result->second.expired())
        return result->second.lock();

    auto scene = GetScene(path);

    if(scene->mNumMeshes == 0)
        throw std::runtime_error("File: " + path + " does not contain any meshes");

    auto mesh = LoadMeshData(scene->mMeshes, scene->mNumMeshes);
    loadedMeshes.insert_or_assign(path, mesh);

    return mesh;
}

std::shared_ptr<Mesh> FileSystem::LoadMeshData(aiMesh** assimpMeshes, unsigned int numMeshes)
{
    auto mesh = std::make_shared<Mesh>();
    mesh->subMeshes.reserve(numMeshes);

    for (unsigned int index = 0; index < numMeshes; index++)
    {
        mesh->subMeshes.push_back(std::move(LoadSubMeshData(assimpMeshes[index])));
    }

    return mesh;
}

Mesh::SubMesh FileSystem::LoadSubMeshData(aiMesh* assimpMesh)
{
    Mesh::SubMesh subMesh;
    subMesh.vertices.reserve(assimpMesh->mNumVertices);
    subMesh.normals.reserve(assimpMesh->mNumVertices);
    subMesh.tangents.reserve(assimpMesh->mNumVertices);
    subMesh.texCoords.reserve(assimpMesh->mNumVertices);
    subMesh.indices.reserve(assimpMesh->mNumFaces);

    for (unsigned int index = 0; index < assimpMesh->mNumVertices; index++)
    {
        subMesh.vertices.emplace_back(glm::vec3(
            assimpMesh->mVertices[index].x,
            assimpMesh->mVertices[index].y,
            assimpMesh->mVertices[index].z));

        subMesh.normals.emplace_back(glm::vec3(
            assimpMesh->mNormals[index].x,
            assimpMesh->mNormals[index].y,
            assimpMesh->mNormals[index].z));

        subMesh.tangents.emplace_back(glm::vec3(
            assimpMesh->mTangents[index].x,
            assimpMesh->mTangents[index].y,
            assimpMesh->mTangents[index].z));

        subMesh.bitangents.emplace_back(glm::vec3(
            assimpMesh->mBitangents[index].x,
            assimpMesh->mBitangents[index].y,
            assimpMesh->mBitangents[index].z));

        if (assimpMesh->HasTextureCoords(0))
        {
            subMesh.texCoords.emplace_back(glm::vec2(
                assimpMesh->mTextureCoords[0][index].x,
                assimpMesh->mTextureCoords[0][index].y));
        }
        else
            subMesh.texCoords.emplace_back(glm::vec2(0, 0));
    }

    for (unsigned int index = 0; index < assimpMesh->mNumFaces; index++)
    {
        subMesh.indices.emplace_back(glm::uvec3(
            assimpMesh->mFaces[index].mIndices[0],
            assimpMesh->mFaces[index].mIndices[1],
            assimpMesh->mFaces[index].mIndices[2]));
    }

    return subMesh;
}

std::vector<std::shared_ptr<Material>> FileSystem::LoadMaterials(const std::string& path)
{
    auto result = loadedMaterials.find(path);
    if (result != loadedMaterials.end())
    {
        auto expired = std::any_of(std::begin(result->second), std::end(result->second), [](const auto& weakResult)
        {
            return weakResult.expired();
        });
        if (!expired)
        {
            std::vector<std::shared_ptr<Material>> results;
            for (const auto& weakResult : result->second)
            {
                results.push_back(weakResult.lock());
            }
            return results;
        }
    }

    auto scene = GetScene(path);

    if (scene->mNumMaterials == 0)
        throw std::runtime_error("File: " + path + " does not contain any materials");

	auto folderEnd = path.find_last_of("\\/");
	std::string folder;
	if(folderEnd != std::string::npos)
		folder = path.substr(0, path.find_last_of("\\/") + 1);
    auto materials = LoadMaterialsData(scene->mMaterials, scene->mNumMaterials, folder);

    std::vector<std::shared_ptr<Material>> orderedMaterials;
    orderedMaterials.reserve(scene->mNumMeshes);
    for (unsigned int index = 0; index < scene->mNumMeshes; index++)
    {
        orderedMaterials.push_back(materials[scene->mMeshes[index]->mMaterialIndex]);
    }

    loadedMaterials.insert_or_assign(path, std::vector<std::weak_ptr<Material>>());
    for (const auto& material : orderedMaterials)
        loadedMaterials[path].push_back(material);

    return orderedMaterials;
}

std::vector<std::shared_ptr<Material>> FileSystem::LoadMaterialsData(aiMaterial** assimpMaterials, unsigned int numMaterials, const std::string& folder)
{
    auto materials = std::vector<std::shared_ptr<Material>>();

    for (unsigned int index = 0; index < numMaterials; index++)
    {
        materials.push_back(std::make_shared<Material>(LoadMaterialData(assimpMaterials[index], folder)));
    }

    return materials;
}

Material FileSystem::LoadMaterialData(aiMaterial* assimpMaterial, const std::string& folder)
{
    Material material;

    if (!LoadMaterialTexture(assimpMaterial, aiTextureType_DIFFUSE, material.Diffuse, folder))
        LoadMaterialColor(assimpMaterial, AI_MATKEY_COLOR_DIFFUSE, material.Diffuse, aiColor4D(1.0f));

    if(!LoadMaterialTexture(assimpMaterial, aiTextureType_NORMALS, material.Normal, folder))
		material.Normal.SetColor(glm::vec3(0.5f, 0.5f, 1.0f));
    
    if (!LoadMaterialTexture(assimpMaterial, aiTextureType_SPECULAR, material.Specular, folder))
        LoadMaterialColor(assimpMaterial, AI_MATKEY_COLOR_SPECULAR, material.Specular, aiColor4D(1.0f, 1.0f, 1.0f, 0.2f));

    if(!LoadMaterialTexture(assimpMaterial, aiTextureType_EMISSIVE, material.Emission, folder))
        LoadMaterialColor(assimpMaterial, AI_MATKEY_COLOR_EMISSIVE, material.Emission, aiColor4D(0.0f));

    LoadMaterialTexture(assimpMaterial, aiTextureType_HEIGHT, material.Height, folder);

    return material;
}

bool FileSystem::LoadMaterialTexture(aiMaterial* assimpMaterial, aiTextureType textureType, Texture& textureData, const std::string& folder)
{
    if (assimpMaterial->GetTextureCount(textureType) != 0)
    {
        aiString texturePath;
        if (assimpMaterial->GetTexture(textureType, 0, &texturePath, nullptr, nullptr, nullptr, nullptr, nullptr) != AI_SUCCESS)
            throw std::runtime_error("Failed to load texture from material");

        textureData = LoadTexture(parentFolder + folder + texturePath.C_Str());

        return true;
    }

    return false;
}

void FileSystem::LoadMaterialColor(aiMaterial* assimpMaterial, const char* pKey, unsigned int type, unsigned int index, Texture& textureData, aiColor4D defaultColor)
{
    aiColor4D color;
	if (aiGetMaterialColor(assimpMaterial, pKey, type, index, &color) != AI_SUCCESS)
		color = defaultColor;
     
	textureData.SetColor(glm::vec4(color.r, color.g, color.b, color.a));
}

Texture FileSystem::LoadTexture(const std::string& path)
{
    Texture texture;

    auto textureFormat = FreeImage_GetFileType(path.c_str(), 0);
    if (textureFormat == FIF_UNKNOWN)
        textureFormat = FreeImage_GetFIFFromFilename(path.c_str());
    if (!textureFormat)
        throw std::runtime_error("Failed to load texture (" + path + "): unknown format");
    if (!FreeImage_FIFSupportsReading(textureFormat))
        throw std::runtime_error("Failed to load texture (" + path + "): texture format is not supported");

    auto loadedTexture = FreeImage_Load(textureFormat, path.c_str());
    if (!loadedTexture)
        throw std::runtime_error("Failed to load texture (" + path + ")");

    auto textureData = FreeImage_GetBits(loadedTexture);
    texture.width = FreeImage_GetWidth(loadedTexture);
    texture.height = FreeImage_GetHeight(loadedTexture);
    texture.bytesPerPixel = FreeImage_GetBPP(loadedTexture) / 8;
    texture.data.assign(textureData, textureData + texture.bytesPerPixel * texture.width * texture.height);

    FreeImage_Unload(loadedTexture);

    return texture;
}
