#pragma once
#include "ShaderFactory.h"

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
    friend class EnvironmentalMap;

    void Load();
    void Unload();

    void BindTexture();

    void SetAlphaColor(float alpha);
    void AddAlphaChannel(float alpha);

    static void LoadQuadMesh(GLuint* vao, GLuint* vbo);
    static void UnloadQuadMesh(GLuint* vao, GLuint* vbo);
    void RenderIntoHDRTexture(float width, float height, ShaderFactory::Type shaderType, GLuint format);
    static void GenerateFrameBuffer(GLuint* fbo, GLuint* rbo, int width, int height);
    static void DeleteFrameBuffer(GLuint* fbo, GLuint* rbo);

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
