#pragma once
#include "ShaderCompiler.h"

class Shader
{
public:
    virtual void setup() = 0;
    virtual void draw(int count) = 0;
    void SetUniformFloat(const std::string& name, float value);
    void SetUniformInt(const std::string& name, int value);
    void SetUniformIVec2(const std::string& name, glm::ivec2 value);
    void SetUniformMat4Array(const std::string name, const std::vector<glm::mat4>& value);

protected:
    GLuint program;

private:
    GLuint GetUniformLocation(const std::string& name);

    std::unordered_map<std::string, GLuint> uniforms;
};
