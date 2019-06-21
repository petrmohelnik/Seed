#include "Shader.h"

void Shader::SetUniformFloat(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformInt(const std::string & name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

GLuint Shader::GetUniformLocation(const std::string& name)
{
    if (auto uniform = uniforms.find(name) != uniforms.end())
        return uniform;

    auto uniform = glGetUniformLocation(program, name.c_str());;
    uniforms.insert({ name, uniform });

    return uniform;
}
