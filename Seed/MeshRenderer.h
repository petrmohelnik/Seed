#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"

class MeshRenderer : public Renderer
{
public:
    MeshRenderer(Object* object);

    void AddToRenderQueue(RenderQueue* queue) override;
	void Render(int index) override;

    void SetMesh(std::shared_ptr<Mesh> mesh_);
    std::shared_ptr<Mesh> GetMesh();
    std::shared_ptr<Mesh> GetSharedMesh();

    void SetMaterial(int index, std::shared_ptr<Material> material);
    void SetMaterials(std::vector<std::shared_ptr<Material>> materials_);
    std::shared_ptr<Material> GetMaterial();
    std::shared_ptr<Material> GetSharedMaterial();
    std::vector<std::shared_ptr<Material>> GetMaterials();
    std::vector<std::shared_ptr<Material>> GetSharedMaterials();
	void Load(const std::string& path);

private:
    struct ModelBlock
    {
        glm::mat4 model;
        glm::mat4 tiModel;
    };

    std::shared_ptr<Mesh> mesh;
    std::vector<std::shared_ptr<Material>> materials;
};
