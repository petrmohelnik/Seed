#pragma once

class Shader;

class Shaders
{
public:
    enum class Type
    {
        Basic
    };

    Shaders();

    Shader* GetShader(Type type);

private:
    void CreateShader(Type type);

    std::unordered_map<Type, std::unique_ptr<Shader>> shaders;
};
