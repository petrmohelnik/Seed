#include "ShaderFactory.h"
#include "BasicShader.h"

ShaderFactory::ShaderFactory()
{
}

Shader* ShaderFactory::GetShader(Type type)
{
    if (shaders.find(type) == shaders.end())
    {
        CreateShader(type);
    }

    return shaders[type].get();
}

void ShaderFactory::CreateShader(Type type)
{
    if (type == Type::Basic)
    {
        shaders.insert({ type , std::make_unique<BasicShader>("Basic.vert", "Basic.frag") });
    }
    else
    {
        throw std::runtime_error("Unsupported shader type");
    }
}
