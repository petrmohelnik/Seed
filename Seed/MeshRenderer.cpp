#include "MeshRenderer.h"
#include "Transform.h"
#include "RenderingPipeline.h"
#include "RenderQueue.h"
#include "ShaderFactory.h"
#include "Engine.h"

MeshRenderer::MeshRenderer(Object* object)
    : Renderer(object)
{
    materials.push_back(std::make_shared<Material>());
    mesh = std::make_shared<Mesh>();
}

void MeshRenderer::AddToRenderQueue(RenderQueue* queue)
{
    for (int index = 0; index < mesh->NumberOfSubmeshes(); index++)
    {
        queue->Add(this, index);
    }
}

void MeshRenderer::Render(int index)
{
    materials[index]->BindMaterial();
    auto shader = Engine::GetShaderFactory().GetShader(materials[index]->GetShader());
    shader->setup();

    RenderingPipeline::BindModelUniform();
    ModelBlock modelBlockData
    { 
        GetTransform()->GetModelMatrix(), 
        glm::mat4(glm::inverse(glm::transpose(GetTransform()->GetModelMatrix()))) 
    };
    glBufferData(GL_UNIFORM_BUFFER, sizeof(modelBlockData), &modelBlockData, GL_DYNAMIC_DRAW);

    mesh->BindSubMesh(index);

    shader->draw(mesh->SubmeshIndicesCount(index));
}

void MeshRenderer::SetMesh(std::shared_ptr<Mesh> mesh_)
{
    mesh = mesh_;
    mesh->Load();
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
    material->Load();
}

void MeshRenderer::SetMaterials(std::vector<std::shared_ptr<Material>> materials_)
{
    materials = materials_;
    for (const auto& material : materials)
    {
        material->Load();
    }
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
