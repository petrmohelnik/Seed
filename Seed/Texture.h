#pragma once

class Texture
{
public:
    Texture();
    Texture(glm::vec4 color);
    ~Texture();

    void SetColor(glm::vec4 color);
    void SetColor(glm::vec3 color);
    void SetColor(glm::vec2 color);
    void SetColor(float color);

protected:
    friend class FileSystem;
    friend class Material;
    friend class TextureCubeMap;

    void Load();
    void Unload();

    void BindTexture();

    void SetAlphaColor(float alpha);
    void AddAlphaChannel(float alpha);

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
