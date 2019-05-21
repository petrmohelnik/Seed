#pragma once
#include "Texture.h"
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

    void SetShader(ShaderFactory::Type shader_);
    ShaderFactory::Type GetShader();

    Texture Diffuse;
    Texture Specular;
    Texture Normal;
    Texture Emission;
    Texture Height;

protected:
    friend class FileSystem;

private:
    struct MaterialBlock
    {
        glm::vec4 placeholder;
    };

    ShaderFactory::Type shader = ShaderFactory::Type::Basic;
};
