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

    void SetShader(ShaderFactory::Type shader_);

protected:
    friend class FileSystem;

private:
    Texture diffuse;
    Texture specular;
    Texture normal;
    Texture height;

    ShaderFactory::Type shader = ShaderFactory::Type::Basic;
};
