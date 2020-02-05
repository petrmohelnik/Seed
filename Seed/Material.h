#pragma once
#include "Texture.h"
#include "TextureCubeMap.h"
#include "ShaderFactory.h"

class Material
{
public:
    Material();
    ~Material();
    Material(Material&&) = default;
    Material(const Material&) = delete;
    Material& operator=(Material&&) = default;
    Material& operator=(const Material&) = delete;
    std::shared_ptr<Material> Copy();

    void Load();
    void Unload();

    void BindMaterial();

    void SetShader(ShaderFactory::Type shader);
    ShaderFactory::Type GetShader();
    void SetSpecularWorkflow();
    void SetMetallicWorkflow();
    void UseOcclusionMap();
    void SetParallaxStrength(float strength);

    std::shared_ptr<Texture> Albedo;
    std::shared_ptr<Texture> Normal;
    std::shared_ptr<Texture> Height;
    //R - ao, G - rougness, B - metallic !Note: when setting channels directly in RGB, use SetIsRGBOrder, default is BGR!
    //For speculaR workflow: RGB - specular color, W - rougness
    std::shared_ptr<Texture> Metallic;
    std::shared_ptr<Texture> Emission;
    std::shared_ptr<Texture> Occlusion; //by specular workflow used by deault

protected:
    friend class FileSystem;

private:
    struct MaterialBlock
    {
        alignas(4) Uint32 SpecularWorkflow = false;
        alignas(4) Uint32 UseOcclusionMap = false;
        alignas(4) float ParallaxStrength = 0.05f;
    };

    MaterialBlock dataBlock;
    ShaderFactory::Type shader = ShaderFactory::Type::GBuffer;
};
