#pragma once
#include "ShaderFactory.h"
#include "Camera.h"
#include "RenderingPipeline.h"

class Texture;
class Framebuffer;

class TextureCubeMap
{
public:
    TextureCubeMap();
    TextureCubeMap(glm::vec4 color);
    ~TextureCubeMap();
    TextureCubeMap(TextureCubeMap&& m) = default;
    TextureCubeMap(const TextureCubeMap& m) = default;
    TextureCubeMap& operator=(const TextureCubeMap& m) = default;
    TextureCubeMap& operator=(TextureCubeMap&& m) = default;

protected:
    friend class FileSystem;
    friend class Material;
    friend class Skybox;
    friend class RenderingPipeline;
    friend class EnvironmentalMap;

    void Load();
    void LoadFromEquirectangular(float *data, int width, int height);
    void Unload();

    void Bind();

    void DefineTexture(GLuint internalFormat, int width, int height, bool generateMipMaps = false, std::vector<const void*> pixels = std::vector<const void*>(6, nullptr), GLuint format = 0, GLuint type = 0);
    void GenerateTexture(bool generateMipMaps = false);
    static std::vector<glm::mat4> GenerateCameraViewsForCube(glm::vec3 position = glm::vec3(0.0f));
    void RenderIntoHDRCubeMapFromTexture(int width, ShaderFactory::Type shaderType, RenderingPipeline::TextureSlot textureSourceSlot, GLuint textureSourceType, GLuint textureSource,
        bool generateMipMaps = false, int mipLevels = 0);
    void ConvertFromHDRToSRGB();

private:
    void DefineTexture();
    void AllocateTexture(GLuint internalFormat, int mipMapLevels, int width, int height, GLuint format, GLuint type, std::vector<const void*> pixels = std::vector<const void*>(6, nullptr));
    void RenderViewsIntoCubeMap(Shader* shader, const Framebuffer& framebuffer);
    void RenderViewsIntoCubeMapWithMipMaps(Shader* shader, Framebuffer& framebuffer, int mipLevels, int width);

    std::vector<std::shared_ptr<Texture>> faces; //right, left, top, bottom, front, back

    bool deleteAfterLoad = true;

    GLuint texture = 0;
    int width;
};
