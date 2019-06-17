#include "TextureCubeMap.h"
#include "Texture.h"
#include "Engine.h"
#include "SimpleMesh.h"
#include "Framebuffer.h"

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

    GenerateTexture();
    DefineTexture();

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

void TextureCubeMap::Bind()
{
    Load();
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
}

void TextureCubeMap::GenerateTexture(bool generateMipMaps)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, generateMipMaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void TextureCubeMap::DefineTexture()
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
    GenerateTexture(generateMipMaps);
    DefineTexture(GL_RGB16F, width, width, GL_RGB, GL_FLOAT, generateMipMaps);

    Framebuffer framebuffer(width, width);

    auto shader = Engine::GetShaderFactory().GetShader(shaderType);
    shader->setup();

    glActiveTexture(textureSourceSlot);
    glBindTexture(textureSourceType, textureSource);

    framebuffer.Bind();
    if(generateMipMaps)
        RenderViewsIntoCubeMapWithMipMaps(shader, framebuffer, mipLevelUniformLocation, mipLevels, width);
    else
        RenderViewsIntoCubeMap(shader, framebuffer);
    framebuffer.Unbind();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubeMap::RenderViewsIntoCubeMap(Shader* shader, const Framebuffer& framebuffer)
{
    SimpleMesh cube(SimpleMesh::Shape::Cube);

    Camera::CameraBlock camera;
    camera.projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    auto const captureViews = GenerateCameraViewsForCube();

    for (unsigned int i = 0; i < 6; i++)
    {
        camera.view = captureViews[i];
        RenderingPipeline::BindCameraUniform();
        glBufferData(GL_UNIFORM_BUFFER, sizeof(camera), &camera, GL_DYNAMIC_DRAW);

        framebuffer.AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube.Bind();
        shader->draw(36);
    }
}

void TextureCubeMap::RenderViewsIntoCubeMapWithMipMaps(Shader* shader, Framebuffer& framebuffer, GLuint mipLevelUniformLocation, int mipLevels, int width)
{
    SimpleMesh cube(SimpleMesh::Shape::Cube);

    Camera::CameraBlock camera;
    camera.projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    auto const captureViews = GenerateCameraViewsForCube();

    for (unsigned int mipLevel = 0; mipLevel < mipLevels; mipLevel++)
    {
        unsigned int mipWidth = width * std::pow(0.5, mipLevel);
        framebuffer.ChangeSize(mipWidth, mipWidth);
        framebuffer.Bind();

        float mipLevelUniform = static_cast<float>(mipLevel) / static_cast<float>(mipLevels - 1);
        glUniform1f(mipLevelUniformLocation, mipLevelUniform);

        for (unsigned int i = 0; i < 6; i++)
        {
            camera.view = captureViews[i];
            RenderingPipeline::BindCameraUniform();
            glBufferData(GL_UNIFORM_BUFFER, sizeof(camera), &camera, GL_DYNAMIC_DRAW);

            framebuffer.AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, mipLevel);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            cube.Bind();
            shader->draw(36);
        }
    }
}

void TextureCubeMap::DefineTexture(GLuint internalFormat, int width, int height, GLuint format, GLuint type, bool generateMipMaps, const void* pixels)
{
    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type, pixels);
    }
    if (generateMipMaps)
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}
