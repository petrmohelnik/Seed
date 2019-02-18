#include "FileSystem.h"
#include <FreeImage.h>
#include "Engine.h"

FileSystem::FileSystem()
{
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType::aiPrimitiveType_LINE | aiPrimitiveType::aiPrimitiveType_POINT);
}

std::string FileSystem::LoadTextFile(const std::string& path)
{
    char *buffer;

    std::ifstream file(path);
    if (file.is_open())
    {
        file.seekg(0, file.end);
        auto length = static_cast<int>(file.tellg());
        file.seekg(0, file.beg);

        buffer = new char[length + 1];

        file.read(buffer, length);
        file.close();

        buffer[length] = '\0';

        std::cout << "file " << path << " loaded" << std::endl;
    }
    else
    {
        throw std::runtime_error("Unable to open file " + path);
    }

    return std::string(buffer);
}

void FileSystem::LoadScene(const std::string& path)
{
    if (loadedScene == path)
        return;

    auto scene = importer.ReadFile(folder + path, 
        aiProcess_GenSmoothNormals | 
        aiProcess_SortByPType | 
        aiProcess_Triangulate);

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

std::vector<std::shared_ptr<Material>> FileSystem::LoadMaterials()
{
    auto scene = importer.GetScene();
    return LoadMaterialsData(scene->mMaterials, scene->mNumMaterials);
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
    mesh->subMeshes.resize(numMeshes);

    for (unsigned int index = 0; index < numMeshes; index++)
    {
        mesh->subMeshes[assimpMeshes[index]->mMaterialIndex] = std::move(LoadSubMeshData(assimpMeshes[index]));
    }

    return mesh;
}

Mesh::SubMesh FileSystem::LoadSubMeshData(aiMesh* assimpMesh)
{
    Mesh::SubMesh subMesh;
    subMesh.vertices.reserve(assimpMesh->mNumVertices);
    subMesh.normals.reserve(assimpMesh->mNumVertices);
    subMesh.texCoords.reserve(assimpMesh->mNumVertices);
    subMesh.indices.reserve(assimpMesh->mNumFaces);

    for (unsigned int index = 0; index < assimpMesh->mNumVertices; index++)
    {
        subMesh.vertices.push_back(glm::vec3(
            assimpMesh->mVertices[index].x,
            assimpMesh->mVertices[index].y,
            assimpMesh->mVertices[index].z));

        subMesh.normals.push_back(glm::vec3(
            assimpMesh->mNormals[index].x,
            assimpMesh->mNormals[index].y,
            assimpMesh->mNormals[index].z));

        if (assimpMesh->HasTextureCoords(0))
        {
            subMesh.texCoords.push_back(glm::vec2(
                assimpMesh->mTextureCoords[0][index].x,
                assimpMesh->mTextureCoords[0][index].y));
        }
        else
            subMesh.texCoords.push_back(glm::vec2(0, 0));
    }

    for (unsigned int index = 0; index < assimpMesh->mNumFaces; index++)
    {
        subMesh.indices.push_back(glm::uvec3(
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

    auto materials = LoadMaterialsData(scene->mMaterials, scene->mNumMaterials);

    loadedMaterials.insert_or_assign(path, std::vector<std::weak_ptr<Material>>());
    for (const auto& material : materials)
        loadedMaterials[path].push_back(material);

    return materials;
}

std::vector<std::shared_ptr<Material>> FileSystem::LoadMaterialsData(aiMaterial** assimpMaterials, unsigned int numMaterials)
{
    auto materials = std::vector<std::shared_ptr<Material>>();

    for (unsigned int index = 0; index < numMaterials; index++)
    {
        materials.push_back(std::make_shared<Material>(LoadMaterialData(assimpMaterials[index])));
    }

    return materials;
}

Material FileSystem::LoadMaterialData(aiMaterial* assimpMaterial)
{
    Material material;

    if (assimpMaterial->GetTextureCount(aiTextureType_DIFFUSE) != 0)
    {
        aiString texturePath;
        if(assimpMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath, nullptr, nullptr, nullptr, nullptr, nullptr) != AI_SUCCESS)
            throw std::runtime_error("Failed to load texture from material");

        material.diffuse = LoadTexture(folder + texturePath.C_Str());
    }
    if (assimpMaterial->GetTextureCount(aiTextureType_NORMALS) != 0)
    {
        aiString texturePath;
        if (assimpMaterial->GetTexture(aiTextureType_NORMALS, 0, &texturePath, nullptr, nullptr, nullptr, nullptr, nullptr) != AI_SUCCESS)
            throw std::runtime_error("Failed to load texture from material");

        material.normal = LoadTexture(folder + texturePath.C_Str());
    }
    if (assimpMaterial->GetTextureCount(aiTextureType_SPECULAR) != 0)
    {
        aiString texturePath;
        if (assimpMaterial->GetTexture(aiTextureType_SPECULAR, 0, &texturePath, nullptr, nullptr, nullptr, nullptr, nullptr) != AI_SUCCESS)
            throw std::runtime_error("Failed to load texture from material");

        material.specular = LoadTexture(folder + texturePath.C_Str());
    }
    if (assimpMaterial->GetTextureCount(aiTextureType_HEIGHT) != 0)
    {
        aiString texturePath;
        if (assimpMaterial->GetTexture(aiTextureType_HEIGHT, 0, &texturePath, nullptr, nullptr, nullptr, nullptr, nullptr) != AI_SUCCESS)
            throw std::runtime_error("Failed to load texture from material");

        material.height = LoadTexture(folder + texturePath.C_Str());
    }

    return material;
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
	texture.bytesPerPixel = FreeImage_GetMemorySize(loadedTexture) / 8;
    texture.data.assign(textureData, textureData + FreeImage_GetMemorySize(loadedTexture));

    FreeImage_Unload(loadedTexture);

    return texture;
}
