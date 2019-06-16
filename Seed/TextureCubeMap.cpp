#include "TextureCubeMap.h"
#include "Texture.h"
#include "Engine.h"

TextureCubeMap::TextureCubeMap()
{
    for (int i = 0; i < 6; i++)
    {
        faces.emplace_back(std::make_shared<Texture>());
    }
}

TextureCubeMap::TextureCubeMap(glm::vec4 color)
{
	for (int i = 0; i < 6; i++)
	{
		faces.emplace_back(std::make_shared<Texture>(color));
	}
}

TextureCubeMap::~TextureCubeMap()
{
    Unload();
}

void TextureCubeMap::Load()
{
    if (texture != 0)
        return;

    GenerateOpenglTexture();
    DefineOpenglTexture();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubeMap::LoadFromEquirectangular(float* data, int width, int height)
{
    if (texture != 0)
        throw std::runtime_error("Texture is already loaded");

    GLuint equirectangularTexture = GenerateEquirectangularTexture(data, width, height);
    
    RenderIntoHDRCubeMapFromTexture(2048, ShaderFactory::Type::EquirectangularToCubemap, GL_TEXTURE6, GL_TEXTURE_2D, equirectangularTexture);

    glDeleteTextures(1, &equirectangularTexture);
}

void TextureCubeMap::Unload()
{
    if (texture == 0)
        return;

    glDeleteTextures(1, &texture);
    texture = 0;
}

void TextureCubeMap::BindTexture()
{
    Load();
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

void TextureCubeMap::LoadCubeMesh(GLuint* vao, GLuint* vbo)
{
    std::vector<glm::vec3> vertices =
    {
        glm::vec3(-1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f,  1.0f, -1.0f),
        glm::vec3(-1.0f,  1.0f, 1.0f),
        glm::vec3(-1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f,  1.0f, 1.0f),
    };

    std::vector<glm::uvec3> indices =
    {
        glm::uvec3(0, 1, 2),
        glm::uvec3(2, 3, 0),
        glm::uvec3(5, 1, 0),
        glm::uvec3(0, 4, 5),
        glm::uvec3(2, 6, 7),
        glm::uvec3(7, 3, 2),
        glm::uvec3(5, 4, 7),
        glm::uvec3(7, 6, 5),
        glm::uvec3(0, 3, 7),
        glm::uvec3(7, 4, 0),
        glm::uvec3(1, 5, 6),
        glm::uvec3(6, 2, 1)
    };

    glGenBuffers(2, vbo);
    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, &vertices[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Uint32) * 3, &indices[0].x, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void TextureCubeMap::UnloadCubeMesh(GLuint* vao, GLuint* vbo)
{
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, vao);
}

void TextureCubeMap::GenerateOpenglTexture(bool generateMipMaps)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, generateMipMaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TextureCubeMap::DefineOpenglTexture()
{
    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, faces[i]->GetInternalFormat(), faces[i]->width, faces[i]->height, 0, faces[i]->GetFormat(), GL_UNSIGNED_BYTE, &faces[i]->data[0]);
        if (deleteAfterLoad)
            faces[i]->data.clear(); faces[i]->data.shrink_to_fit();
    }
}

GLuint TextureCubeMap::GenerateEquirectangularTexture(float* data, int width, int height)
{
    GLuint equirectangularTexture;
    glGenTextures(1, &equirectangularTexture);
    glBindTexture(GL_TEXTURE_2D, equirectangularTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

    return equirectangularTexture;
}

std::vector<glm::mat4> TextureCubeMap::GenerateCameraViewsForCube()
{
    return std::vector<glm::mat4>
    {
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
}

void TextureCubeMap::RenderIntoHDRCubeMapFromTexture(int width, ShaderFactory::Type shaderType, GLuint textureSourceSlot, GLuint textureSourceType, GLuint textureSource,
    bool generateMipMaps, GLuint mipLevelUniformLocation, int mipLevels)
{
    GenerateOpenglTexture(generateMipMaps);
    DefineOpenglTexture(GL_RGB16F, width, width, GL_RGB, GL_FLOAT, generateMipMaps);

    GLuint vao, vbo[2];
    LoadCubeMesh(&vao, &vbo[0]);

    GLuint fbo, rbo;
    Texture::GenerateFrameBuffer(&fbo, &rbo, width, width);

    auto shader = Engine::GetShaderFactory().GetShader(shaderType);
    shader->setup();

    glActiveTexture(textureSourceSlot);
    glBindTexture(textureSourceType, textureSource);

    glViewport(0, 0, width, width);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    if(generateMipMaps)
        RenderViewsIntoCubeMapWithMipMaps(vao, rbo, shader, mipLevelUniformLocation, mipLevels, width);
    else
        RenderViewsIntoCubeMap(vao, shader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Engine::GetWindow().ResetViewport();

    UnloadCubeMesh(&vao, &vbo[0]);
    Texture::DeleteFrameBuffer(&fbo, &rbo);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubeMap::RenderViewsIntoCubeMap(GLuint vao, Shader* shader)
{
    Camera::CameraBlock camera;
    camera.projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    auto const captureViews = GenerateCameraViewsForCube();

    for (unsigned int i = 0; i < 6; i++)
    {
        camera.view = captureViews[i];
        RenderingPipeline::BindCameraUniform();
        glBufferData(GL_UNIFORM_BUFFER, sizeof(camera), &camera, GL_DYNAMIC_DRAW);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        shader->draw(36);
    }
}

void TextureCubeMap::RenderViewsIntoCubeMapWithMipMaps(GLuint vao, GLuint rbo, Shader* shader, GLuint mipLevelUniformLocation, int mipLevels, int width)
{
    Camera::CameraBlock camera;
    camera.projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    auto const captureViews = GenerateCameraViewsForCube();

    for (unsigned int mipLevel = 0; mipLevel < mipLevels; mipLevel++)
    {
        unsigned int mipWidth = width * std::pow(0.5, mipLevel);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipWidth);
        glViewport(0, 0, mipWidth, mipWidth);

        float mipLevelUniform = static_cast<float>(mipLevel) / static_cast<float>(mipLevels - 1);
        glUniform1f(mipLevelUniformLocation, mipLevelUniform);

        for (unsigned int i = 0; i < 6; i++)
        {
            camera.view = captureViews[i];
            RenderingPipeline::BindCameraUniform();
            glBufferData(GL_UNIFORM_BUFFER, sizeof(camera), &camera, GL_DYNAMIC_DRAW);

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, mipLevel);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(vao);
            shader->draw(36);
        }
    }
}

void TextureCubeMap::DefineOpenglTexture(GLuint internalFormat, int width, int height, GLuint format, GLuint type, bool generateMipMaps, const void* pixels)
{
    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type, pixels);
    }
    if (generateMipMaps)
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}
