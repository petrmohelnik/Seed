#include "MeshRenderer.h"
#include "Transform.h"
#include "RenderingPipeline.h"
#include "RenderQueue.h"
#include "ShaderFactory.h"
#include "Engine.h"

AABB MeshRenderer::GetAABB(int index)
{
    return mesh->subMeshes[index]->GetAABB().GetTransformedAABB(GetTransform()->GetModelMatrix());
}

void MeshRenderer::AddToRenderQueueDeferred(RenderQueue& queue)
{
    for (int index = 0; index < mesh->NumberOfSubmeshes(); index++)
    {
        if(materials[index]->GetShader() == ShaderFactory::Type::GBuffer)
            queue.Add(this, index);
    }
}

void MeshRenderer::AddToRenderQueueForward(RenderQueue& queue)
{
    for (int index = 0; index < mesh->NumberOfSubmeshes(); index++)
    {
        if (materials[index]->GetShader() != ShaderFactory::Type::GBuffer)
            queue.Add(this, index);
    }
}

void MeshRenderer::Render(int index)
{
    materials[index]->BindMaterial();
    Render(index, materials[index]->GetShader());
}

void MeshRenderer::Render(int index, ShaderFactory::Type shaderType)
{
    auto shader = Engine::GetShaderFactory().GetShader(shaderType);
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

MeshRenderer* MeshRenderer::Clone()
{
     return new MeshRenderer(*this);
}

void MeshRenderer::SetMesh(std::shared_ptr<Mesh> mesh_)
{
    mesh = mesh_;
    mesh->Load();
}

std::shared_ptr<Mesh> MeshRenderer::GetMesh()
{
    mesh = mesh->Copy();
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
    materials[0] = materials[0]->Copy();
    return materials[0];
}

std::shared_ptr<Material> MeshRenderer::GetSharedMaterial()
{
    return materials[0];
}

std::vector<std::shared_ptr<Material>> MeshRenderer::GetMaterials()
{
    for (auto& material : materials)
        material = material->Copy();
    return materials;
}

std::vector<std::shared_ptr<Material>> MeshRenderer::GetSharedMaterials()
{
    return materials;
}

void MeshRenderer::Load(const std::string& path, bool deleteAfterLoad)
{
    SetMesh(Engine::GetFileSystem().LoadMesh(path, deleteAfterLoad));
    SetMaterials(Engine::GetFileSystem().LoadMaterials(path));
    Engine::GetFileSystem().UnloadScene();
}
