#pragma once
#include "Shader.h"

class BasicShader : public Shader
{
public:
    BasicShader(const std::string& vertexShader, const std::string& fragmentShader);

    void setup() override;
    void draw(int count) override;

private:
};
