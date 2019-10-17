#pragma once
#include "Shader.h"

class BasicShader : public Shader
{
public:
    BasicShader(GLuint program);

    void setup() override;
    void draw(int count) override;

private:
};
