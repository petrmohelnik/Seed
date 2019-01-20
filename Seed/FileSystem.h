#pragma once

class Mesh;
class Material;

class FileSystem
{
public:
    static std::shared_ptr<Mesh> LoadMesh(std::string path);
	static std::shared_ptr<Material> LoadMaterial(std::string path);
    static std::shared_ptr<Mesh> CreateSphere();
    static std::shared_ptr<Mesh> CreatePlane();

private:
    static std::unordered_map<std::string, std::weak_ptr<Mesh>> meshes;
	static std::unordered_map<std::string, std::weak_ptr<Material>> materials;
};
