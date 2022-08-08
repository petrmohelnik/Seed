#pragma once
#include "Shader.h"

class TessellationShader : public Shader
{
public:
    TessellationShader(GLuint program);

    void setup() override;
    void draw(int count) override;

private:
};
