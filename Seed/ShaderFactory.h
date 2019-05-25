#pragma once
#include "Shader.h"

class ShaderFactory
{
public:
    enum class Type
    {
        Basic,
        Basic2,
        Basic3,
    };

    ShaderFactory();

    Shader* GetShader(Type type);

private:
    void CreateShader(Type type);

    std::unordered_map<Type, std::unique_ptr<Shader>> shaders;
};
