#pragma once

class Texture
{
public:
    Texture();
    ~Texture();

    void Load();
    void Unload();

protected:
    friend class FileSystem;
    void SetColor(glm::u8vec4 color);

private:
    std::vector<Uint8> data;
    unsigned int width;
    unsigned int height;

    bool deleteAfterLoad = false;

    GLuint sampler;
};
