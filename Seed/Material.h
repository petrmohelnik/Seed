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

    std::shared_ptr<Texture> Diffuse;
    std::shared_ptr<Texture> Normal;
    std::shared_ptr<Texture> Height;
    std::shared_ptr<Texture> Specular;
    std::shared_ptr<Texture> Emission;
    std::shared_ptr<TextureCubeMap> Environmental;

protected:
    friend class FileSystem;

private:
    struct MaterialBlock
    {
        glm::vec4 placeholder;
    };

    ShaderFactory::Type shader = ShaderFactory::Type::Basic;
};
