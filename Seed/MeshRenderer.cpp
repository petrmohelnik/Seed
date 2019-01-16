#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Object* object, Input& input, Time& time)
    : Renderer(object, input, time)
{
    materials.push_back(std::make_shared<Material>());
}

void MeshRenderer::Render()
{
}

void MeshRenderer::SetMesh(std::shared_ptr<Mesh> mesh_)
{
    mesh = mesh;
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
