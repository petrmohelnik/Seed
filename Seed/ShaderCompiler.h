#pragma once

class ShaderCompiler
{
public:
    static GLuint Compile(const std::string& vertexShader, const std::string& fragmentShader);
    static GLuint Compile(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);
    static GLuint Compile(const std::string& vertexShader, const std::string& tessellationControlShader, const std::string& tessellationEvaluationShader, const std::string& fragmentShader);

private:
    static void CompileShader(const std::string& path, GLuint shaderType, GLuint program);
    static void LinkProgram(GLuint program);
};
