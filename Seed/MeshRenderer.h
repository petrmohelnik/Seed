#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"

class MeshRenderer : public Renderer
{
public:
    MeshRenderer(Object* object,
                 Input& input,
                 Time& time);

	void Render() override;

    void SetMesh(std::shared_ptr<Mesh> mesh_);
    std::shared_ptr<Mesh> GetMesh();
    std::shared_ptr<Mesh> GetSharedMesh();

    std::shared_ptr<Material> GetMaterial();
    std::shared_ptr<Material> GetSharedMaterial();
    std::vector<std::shared_ptr<Material>> GetMaterials();
    std::vector<std::shared_ptr<Material>> GetSharedMaterials();

private:
    std::shared_ptr<Mesh> mesh;
    std::vector<std::shared_ptr<Material>> materials;
};
