#pragma once
#include "Texture.h"
#include "Shader.h"

class Material
{
public:
    Material();
    ~Material();

protected:
    friend class FileSystem;

private:
    Texture diffuse;
    Texture specular;
    Texture normal;
    Texture height;

    Shader::Type shader;
};
