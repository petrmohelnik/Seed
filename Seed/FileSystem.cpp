#include "FileSystem.h"
#include "Mesh.h"
#include "Material.h"

void FileSystem::LoadScene(const std::string& path)
{
    if (loadedScene == path)
        return;

    auto scene = importer.ReadFile(folder + path, aiProcess_GenSmoothNormals | aiProcess_SortByPType);
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
    if (result != meshes.end())
        return result->second.lock();

    auto scene = GetScene(path);

    if(scene->mNumMeshes == 0)
        throw std::runtime_error("File: " + path + " does not contain any meshes");

    auto mesh = LoadMeshData(scene->mMeshes[1]);
    meshes.insert({ path, mesh });

    return mesh;
}

std::shared_ptr<Mesh> FileSystem::LoadMeshData(aiMesh* assimpMesh)
{
    auto mesh = std::make_shared<Mesh>();

    for (unsigned int index = 0; index < assimpMesh->mNumVertices; index++)
    {
        mesh->vertices.push_back(glm::vec3(
            assimpMesh->mVertices[index].x, 
            assimpMesh->mVertices[index].y, 
            assimpMesh->mVertices[index].z));

        mesh->normals.push_back(glm::vec3(
            assimpMesh->mNormals[index].x,
            assimpMesh->mNormals[index].y,
            assimpMesh->mNormals[index].z));

        if (assimpMesh->HasTextureCoords)
        {
            mesh->texCoords.push_back(glm::vec2(
                assimpMesh->mTextureCoords[0][index].x,
                assimpMesh->mTextureCoords[0][index].y));
        }
        else
            mesh->texCoords.push_back(glm::vec2(0, 0));

    }

    return std::shared_ptr<Mesh>();
}
