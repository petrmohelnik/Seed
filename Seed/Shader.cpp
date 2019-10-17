#include "Shader.h"

void Shader::SetUniformFloat(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniformInt(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniformIVec2(const std::string& name, glm::ivec2 value)
{
    glUniform2i(GetUniformLocation(name), value.x, value.y);
}

void Shader::SetUniformMat4Array(const std::string name, const std::vector<glm::mat4>& value)
{
    glUniformMatrix4fv(GetUniformLocation(name), value.size(), GL_FALSE, glm::value_ptr(value[0]));
}

GLuint Shader::GetUniformLocation(const std::string& name)
{
    if (uniforms.find(name) != uniforms.end())
        return uniforms[name];

    auto uniform = glGetUniformLocation(program, name.c_str());;
    uniforms.insert({ name, uniform });

    return uniform;
}
