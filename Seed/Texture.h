#pragma once
#include "ShaderFactory.h"

class Texture
{
public:
    Texture();
    Texture(glm::vec4 color);
    ~Texture();
    Texture(Texture&&) = default;
    Texture(const Texture&) = delete;
    Texture& operator=(Texture&&) = default;
    Texture& operator=(const Texture&) = delete;
    std::shared_ptr<Texture> Copy();

    void SetColor(glm::vec4 color);
    void SetColor(glm::vec3 color);
    void SetColor(glm::vec2 color);
    void SetColor(float color);
    void SetIsSRGB();
    void SetIsRGBOrder();
    void SetAlphaColor(float alpha);
    void AddChannel(float value);
    void AddChannelFromTexture(std::shared_ptr<Texture> textureFrom, int channelFrom);

protected:
    friend class FileSystem;
    friend class Material;
    friend class TextureCubeMap;
    friend class EnvironmentalMap;
    friend class RenderingPipeline;

    void Load();
    void Unload();

    void Bind();

    void GenerateTexture(GLuint wrapParam, GLuint internalFormat, int width, int height, bool generateMipMaps = false, const void* pixels = nullptr, GLuint format = 0, GLuint type = 0);
    void RenderIntoHDRTexture(int width, int height, ShaderFactory::Type shaderType, GLuint format);

private:
    void AllocateTexture(GLuint internalFormat, int mipMapLevels, int width, int height, GLuint format, GLuint type, const void* pixels = nullptr);
    GLuint GetInternalFormat();
    GLuint GetFormat();

    std::vector<Uint8> data;
    int width;
    int height;
    int bytesPerPixel;

    bool deleteAfterLoad = true;
    bool isSRGB = false;
    bool isRGB = false;

    GLuint texture = 0;
};
