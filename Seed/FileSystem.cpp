#include "FileSystem.h"

FileSystem::FileSystem()
{
    importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType::aiPrimitiveType_LINE | aiPrimitiveType::aiPrimitiveType_POINT);
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
}

const aiScene* FileSystem::GetScene(const std::string& path)
{
    LoadScene(path);
    return importer.GetScene();
}

std::shared_ptr<Mesh> FileSystem::LoadMesh(const std::string& path)
{
    auto result = meshes.find(path);
    if (result != meshes.end() && !result->second.expired())
        return result->second.lock();

    auto scene = GetScene(path);

    if(scene->mNumMeshes == 0)
        throw std::runtime_error("File: " + path + " does not contain any meshes");

    auto mesh = LoadMeshData(scene->mMeshes, scene->mNumMeshes);
    meshes.insert_or_assign(path, mesh);

    return mesh;
}

std::shared_ptr<Mesh> FileSystem::LoadMeshData(aiMesh** assimpMeshes, unsigned int numMeshes)
{
    auto mesh = std::make_shared<Mesh>();
    mesh->subMeshes.resize(numMeshes);

    for (unsigned int index = 0; index < numMeshes; index++)
    {
        mesh->subMeshes.insert(mesh->subMeshes.begin() + assimpMeshes[index]->mMaterialIndex, LoadSubMeshData(assimpMeshes[index]));
    }

    return std::shared_ptr<Mesh>();
}

Mesh::SubMesh FileSystem::LoadSubMeshData(aiMesh* assimpMesh)
{
    Mesh::SubMesh mesh;

    for (unsigned int index = 0; index < assimpMesh->mNumVertices; index++)
    {
        mesh.vertices.push_back(glm::vec3(
            assimpMesh->mVertices[index].x,
            assimpMesh->mVertices[index].y,
            assimpMesh->mVertices[index].z));

        mesh.normals.push_back(glm::vec3(
            assimpMesh->mNormals[index].x,
            assimpMesh->mNormals[index].y,
            assimpMesh->mNormals[index].z));

        if (assimpMesh->HasTextureCoords(0))
        {
            mesh.texCoords.push_back(glm::vec2(
                assimpMesh->mTextureCoords[0][index].x,
                assimpMesh->mTextureCoords[0][index].y));
        }
        else
            mesh.texCoords.push_back(glm::vec2(0, 0));
    }

    for (unsigned int index = 0; index < assimpMesh->mNumFaces; index++)
    {
        mesh.indices.push_back(glm::uvec3(
            assimpMesh->mFaces[index].mIndices[0],
            assimpMesh->mFaces[index].mIndices[1],
            assimpMesh->mFaces[index].mIndices[2]));
    }

    return mesh;
}

std::vector<std::shared_ptr<Material>> FileSystem::LoadMaterials(const std::string& path)
{
    auto result = materials.find(path);
    if (result != materials.end())
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

    auto material = scene->mMaterials[1];
    auto count = material->GetTextureCount(aiTextureType_DIFFUSE);

    return std::vector<std::shared_ptr<Material>>();
}
