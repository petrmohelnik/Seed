#include "FileSystem.h"
#include "Mesh.h"

std::unordered_map<std::string, std::weak_ptr<Mesh>> FileSystem::meshes;

std::shared_ptr<Mesh> FileSystem::LoadFromFile(std::string path)
{
    auto mesh = meshes.find(path);
    if (mesh != meshes.end())
        return mesh->second.lock();

    return std::make_shared<Mesh>();
}
