#pragma once
#include "Texture.h"
#include "Shader.h"

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

protected:
    friend class FileSystem;

private:
    Texture diffuse;
    Texture specular;
    Texture normal;
    Texture height;

    Shader* shader;
};
