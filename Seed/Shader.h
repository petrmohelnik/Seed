#pragma once
#include "ShaderCompiler.h"

class Shader
{
public:
    virtual void setup() = 0;
    virtual void draw(int count) = 0;
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformInt(const std::string& name, int value);

private:
    GLuint GetUniformLocation(const std::string& name);

    GLuint program;

    std::unordered_map<std::string, GLuint> uniforms;
};
