#pragma once

class Texture
{
public:
    Texture();
    ~Texture();

    void Load();
    void Unload();

private:
    std::vector<Uint8> tex;
    unsigned int width;
    unsigned int height;

    GLuint sampler;
};
