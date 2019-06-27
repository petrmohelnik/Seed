#pragma once
#include "ShaderFactory.h"

class Texture
{
public:
    Texture();
    Texture(glm::vec4 color);
    ~Texture();
    Texture(Texture&& m) = default;
    Texture(const Texture& m) = default;
    Texture& operator=(const Texture& m) = default;
    Texture& operator=(Texture&& m) = default;

    void SetColor(glm::vec4 color);
    void SetColor(glm::vec3 color);
    void SetColor(glm::vec2 color);
    void SetColor(float color);
    void SetSRGB();

protected:
    friend class FileSystem;
    friend class Material;
    friend class TextureCubeMap;
    friend class EnvironmentalMap;
    friend class RenderingPipeline;

    void Load();
    void Unload();

    void Bind();

    void SetAlphaColor(float alpha);
    void AddAlphaChannel(float alpha);

    void GenerateTexture(GLuint wrapParam, GLuint internalFormat, int width, int height, GLuint format, GLuint type, bool generateMipMaps = false, const void* pixels = nullptr);
    void RenderIntoHDRTexture(float width, float height, ShaderFactory::Type shaderType, GLuint format);

private:
	GLuint GetInternalFormat();
	GLuint GetFormat();

    std::vector<Uint8> data;
    unsigned int width;
    unsigned int height;
	unsigned int bytesPerPixel;

    bool deleteAfterLoad = true;
    bool isSRGB = false;

    GLuint texture = 0;
};
