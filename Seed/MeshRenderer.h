#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "Material.h"

class MeshRenderer : public Renderer
{
public:
    using Renderer::Renderer;

    void SetMesh(std::shared_ptr<Mesh> mesh_);
    std::shared_ptr<Mesh> GetMesh();
    std::shared_ptr<Mesh> GetSharedMesh();

    void SetMaterial(int index, std::shared_ptr<Material> material);
    void SetMaterials(std::vector<std::shared_ptr<Material>> materials_);
    std::shared_ptr<Material> GetMaterial();
    std::shared_ptr<Material> GetSharedMaterial();
    std::vector<std::shared_ptr<Material>> GetMaterials();
    std::vector<std::shared_ptr<Material>> GetSharedMaterials();
    void Load(const std::string& path, bool deleteAfterLoad = true);

protected:
    friend class RenderingPipeline;

    AABB GetAABB(int index);
    void AddToRenderQueueDeferred(RenderQueue& queue) override;
    void AddToRenderQueueForward(RenderQueue& queue) override;
    void Render(int index) override;
    void Render(int index, ShaderFactory::Type shaderType) override;

private:
    MeshRenderer* Clone() override;

    struct ModelBlock
    {
        glm::mat4 model;
        glm::mat4 tiModel;
    };

    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
    std::vector<std::shared_ptr<Material>> materials = std::vector<std::shared_ptr<Material>>{ std::make_shared<Material>() };
};
