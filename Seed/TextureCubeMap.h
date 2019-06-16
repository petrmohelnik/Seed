#pragma once
#include "ShaderFactory.h"
#include "Camera.h"

class Texture;

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

    void BindTexture();

    static void LoadCubeMesh(GLuint* vao, GLuint* vbo);
    static void UnloadCubeMesh(GLuint* vao, GLuint* vbo);
    void DefineOpenglTexture(GLuint internalFormat, int width, int height, GLuint format, GLuint type, bool generateMipMaps = false, const void* pixels = nullptr);
    void GenerateOpenglTexture(bool generateMipMaps = false);
    static std::vector<glm::mat4> GenerateCameraViewsForCube();
    void RenderIntoHDRCubeMapFromTexture(int width, ShaderFactory::Type shaderType, GLuint textureSourceSlot, GLuint textureSourceType, GLuint textureSource,
        bool generateMipMaps = false, GLuint mipLevelUniformLocation = 0, int mipLevels = 0);

private:
    void DefineOpenglTexture();
    GLuint GenerateEquirectangularTexture(float* data, int width, int height);
    void RenderViewsIntoCubeMap(GLuint vao, Shader* shader);
    void RenderViewsIntoCubeMapWithMipMaps(GLuint vao, GLuint rbo, Shader* shader, GLuint mipLevelUniformLocation, int mipLevels, int width);

    std::vector<std::shared_ptr<Texture>> faces; //right, left, top, bottom, front, back

    bool deleteAfterLoad = true;

    GLuint texture = 0;
};
