#pragma once

class Shader
{
public:
    enum class Type
    {
        Diffuse
    };

    Shader();

private:
    GLuint program;
};
