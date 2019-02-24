#pragma once

class Texture
{
public:
    Texture();

    void Load();
    void Unload();

    void BindTexture();

protected:
    friend class FileSystem;
    void SetColor(glm::u8vec4 color);
    void SetColor(glm::u8vec3 color);
    void SetColor(glm::u8vec2 color);
    void SetColor(glm::u8vec1 color);

private:
	GLuint GetInternalFormat();
	GLuint GetFormat();

    std::vector<Uint8> data;
    unsigned int width;
    unsigned int height;
	unsigned int bytesPerPixel;

    bool deleteAfterLoad = true;

    GLuint texture = 0;
};
