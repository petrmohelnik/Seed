#include "FileSystem.h"
#include "Mesh.h"

std::unordered_map<std::string, std::weak_ptr<Mesh>> FileSystem::meshes;

std::shared_ptr<Mesh> FileSystem::LoadFromFile(std::string path)
{
    auto mesh = meshes.find(path);
    if (mesh != meshes.end())
        return mesh->second.lock();


    Assimp::Importer importer;

    auto scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

    if (!scene)
        throw std::runtime_error("Failed to parse " + path + "file : " + importer.GetErrorString());

    auto meshes = scene->mMeshes;
    auto materials = scene->mMaterials;
    auto lights = scene->mLights;
    auto cameras = scene->mCameras;
    auto material = materials[0];

    return std::make_shared<Mesh>();
}
