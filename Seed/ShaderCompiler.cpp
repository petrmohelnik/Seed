#include "ShaderCompiler.h"
#include "Engine.h"

GLuint ShaderCompiler::Compile(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();
    CompileShader(vertexShader, GL_VERTEX_SHADER, program);
    CompileShader(fragmentShader, GL_FRAGMENT_SHADER, program);
    LinkProgram(program);
   
    return program;
}

void ShaderCompiler::CompileShader(const std::string& path, GLuint shaderType, GLuint program)
{
    auto file = Engine::GetFileSystem().LoadTextFile(path);

    auto shader = glCreateShader(shaderType);
    const char *c_str = file.c_str();
    glShaderSource(shader, 1, &c_str, NULL);
    glCompileShader(shader);

    int compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    int logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 1) {
        std::string info;
        info.reserve(logLength);
        glGetShaderInfoLog(shader, logLength, nullptr, &info[0]);
        std::cout << "GLSL compiler " << path << ": " << info << std::endl;
    }

    if (compiled != 0)
    {
        throw std::runtime_error("Error compiling shader: " + path);
    }

    glAttachShader(program, shader);
}

void ShaderCompiler::LinkProgram(GLuint program)
{
    glLinkProgram(program);

    int linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    int logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 1) {
        std::string info;
        info.reserve(logLength);
        glGetProgramInfoLog(program, logLength, nullptr, &info[0]);
        std::cout << "GLSL linker: " << info << std::endl;
    }

    if (linked != 0)
    {
        throw std::runtime_error("Error linking program");
    }
}
