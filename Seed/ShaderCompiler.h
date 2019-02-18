#pragma once

class ShaderCompiler
{
public:
    static GLuint Compile(const std::string& vertexShader, const std::string& fragmentShader);

private:
    static void CompileShader(const std::string& path, GLuint shaderType, GLuint program);
    static void LinkProgram(GLuint program);
};
