#pragma once

class Mesh;

class FileSystem
{
public:
    static std::shared_ptr<Mesh> LoadFromFile(std::string path);
    static std::shared_ptr<Mesh> CreateSphere();
    static std::shared_ptr<Mesh> CreatePlane();

private:
    static std::unordered_map<std::string, std::weak_ptr<Mesh>> meshes;
};
