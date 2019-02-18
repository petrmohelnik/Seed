#pragma once
#include "ShaderCompiler.h"

class Shader
{
public:
    virtual void setup() = 0;
    virtual void draw() = 0;

private:
    GLuint program;
};
