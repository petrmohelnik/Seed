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

protected:
    friend class FileSystem;

private:
    struct MaterialBlock
    {
        glm::vec4 placeholder;
    };

    Texture diffuse;
    Texture specular;
    Texture normal;
    Texture height;

    ShaderFactory::Type shader = ShaderFactory::Type::Basic;
};
