#pragma once
#include "Texture.h"
#include "TextureCubeMap.h"
#include "ShaderFactory.h"

class Material
{
public:
    Material();
    ~Material();
    Material(Material&& m) = default;
    Material(const Material& m) = default;
    Material& operator=(const Material& m) = default;
    Material& operator=(Material&& m) = default;

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
    std::shared_ptr<Texture> Metallic;
    std::shared_ptr<Texture> Emission;
    std::shared_ptr<Texture> Occlusion;

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
