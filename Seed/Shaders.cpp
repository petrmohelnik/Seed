#include "Shaders.h"
#include "Shader.h"

Shaders::Shaders()
{
}

Shader* Shaders::GetShader(Type type)
{
    if (shaders.find(type) == shaders.end())
    {
        CreateShader(type);
    }

    return shaders[type].get();
}

void Shaders::CreateShader(Type type)
{
    if (type == Type::Basic)
    {
        shaders.emplace(BasicShader("Basic.vs"));
    }
    else
    {
        throw std::runtime_error("Unsupported shader type");
    }
}
