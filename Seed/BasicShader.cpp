#include "BasicShader.h"

BasicShader::BasicShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    program = ShaderCompiler::Compile(vertexShader, fragmentShader);
}

void BasicShader::setup()
{
    glUseProgram(program);
}

void BasicShader::draw(int count)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}
