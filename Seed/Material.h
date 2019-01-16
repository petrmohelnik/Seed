#pragma once
#include "Texture.h"
#include "Shader.h"

class Material
{
public:
    Material();
    ~Material();

private:
    std::shared_ptr<Texture> diffuse;
    std::shared_ptr<Texture> specular;
    std::shared_ptr<Texture> normal;
    std::shared_ptr<Texture> height;
    glm::vec3 emission;
    Shader::Type shader;
};
