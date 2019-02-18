#include "BasicShader.h"

BasicShader::BasicShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    program = ShaderCompiler::Compile(vertexShader, fragmentShader);
}

void BasicShader::setup()
{
}

void BasicShader::draw()
{
}
