#pragma once

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
    static void GenerateFrameBuffer(GLuint* fbo, GLuint* rbo, int width, int height);
    static void DeleteFrameBuffer(GLuint* fbo, GLuint* rbo);
    void DefineOpenglTexture(GLuint internalFormat, int width, int height, GLuint format, GLuint type, const void* pixels = nullptr);
    void GenerateOpenglTexture();
    static std::vector<glm::mat4> GenerateCameraViewsForCube();

private:
    void DefineOpenglTexture();
    GLuint GenerateEquirectangularTexture(float* data, int width, int height);

    std::vector<std::shared_ptr<Texture>> faces; //right, left, top, bottom, front, back

    bool deleteAfterLoad = true;

    GLuint texture = 0;
};

