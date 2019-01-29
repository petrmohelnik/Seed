#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Object* object)
    : Renderer(object)
{
    materials.push_back(std::make_shared<Material>());
    mesh = std::make_shared<Mesh>();
}

void MeshRenderer::Render()
{
}

void MeshRenderer::SetMesh(std::shared_ptr<Mesh> mesh_)
{
    mesh = mesh_;
}

std::shared_ptr<Mesh> MeshRenderer::GetMesh()
{
    mesh = std::make_shared<Mesh>(*mesh);
    return mesh;
}

std::shared_ptr<Mesh> MeshRenderer::GetSharedMesh()
{
    return mesh;
}

void MeshRenderer::SetMaterial(int index, std::shared_ptr<Material> material)
{
    if (materials.size() <= index)
        materials.resize(index + 1, std::make_shared<Material>());
    materials.insert(materials.begin() + index, material);
}

void MeshRenderer::SetMaterials(std::vector<std::shared_ptr<Material>> materials_)
{
    materials = materials_;
}

std::shared_ptr<Material> MeshRenderer::GetMaterial()
{
    materials[0] = std::make_shared<Material>(*materials[0]);
    return materials[0];
}

std::shared_ptr<Material> MeshRenderer::GetSharedMaterial()
{
    return materials[0];
}

std::vector<std::shared_ptr<Material>> MeshRenderer::GetMaterials()
{
    for (auto& material : materials)
        material = std::make_shared<Material>(*material);
    return materials;
}

std::vector<std::shared_ptr<Material>> MeshRenderer::GetSharedMaterials()
{
    return materials;
}
