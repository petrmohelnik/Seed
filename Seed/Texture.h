#pragma once

class Texture
{
public:
    Texture();
    ~Texture();

    void Load();
    void Unload();

private:
    std::vector<unsigned char> tex;
    unsigned int width;
    unsigned int height;

    GLuint sampler;
};
