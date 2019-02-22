#include "BasicShader.h"

BasicShader::BasicShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    program = ShaderCompiler::Compile(vertexShader, fragmentShader);
    glUniformBlockBinding(program, 0, 0);
    glUniformBlockBinding(program, 1, 1);
    glUniformBlockBinding(program, 2, 2);
    glUniformBlockBinding(program, 3, 3);
}

void BasicShader::setup()
{
}

void BasicShader::draw()
{
}
