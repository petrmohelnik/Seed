#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "SubMesh.h"
#include "Material.h"

class MeshRenderer : public Renderer
{
public:
    MeshRenderer(Object* object);

    void SetMesh(std::shared_ptr<Mesh> mesh_);
    //std::shared_ptr<Mesh> GetMesh();
    std::shared_ptr<Mesh> GetSharedMesh();

    void SetMaterial(int index, std::shared_ptr<Material> material);
    void SetMaterials(std::vector<std::shared_ptr<Material>> materials_);
    //std::shared_ptr<Material> GetMaterial();
    std::shared_ptr<Material> GetSharedMaterial();
    //std::vector<std::shared_ptr<Material>> GetMaterials();
    std::vector<std::shared_ptr<Material>> GetSharedMaterials();
	void Load(const std::string& path);

protected:
    friend class RenderingPipeline;

    AABB GetAABB(int index);
    void AddToRenderQueueDeferred(RenderQueue& queue) override;
    void AddToRenderQueueForward(RenderQueue& queue) override;
	void Render(int index) override;
    void Render(int index, ShaderFactory::Type shaderType) override;

private:
    void CloneMaterialTextures(std::shared_ptr<Material>& material);

    struct ModelBlock
    {
        glm::mat4 model;
        glm::mat4 tiModel;
    };

    std::shared_ptr<Mesh> mesh;
    std::vector<std::shared_ptr<Material>> materials;
};
