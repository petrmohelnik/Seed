#pragma once

class Shader
{
public:
    Shader(const std::string& path);

    virtual void setup() = 0;
    virtual void draw() = 0;

private:
    GLuint program;
};
