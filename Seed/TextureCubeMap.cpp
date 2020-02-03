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

	Texture equirectangularTexture;
	equirectangularTexture.GenerateTexture(GL_CLAMP_TO_EDGE, GL_RGB16F, width, height, false, data, GL_RGB, GL_FLOAT);
    
    RenderIntoHDRCubeMapFromTexture(2048, ShaderFactory::Type::EquirectangularToCubemap, RenderingPipeline::TextureSlot::Environmental, GL_TEXTURE_2D, equirectangularTexture.texture, true, false);

    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
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

void TextureCubeMap::DefineTexture(GLuint internalFormat, int width_, int height, bool generateMipMaps, std::vector<const void*> pixels, GLuint format, GLuint type)
{
    width = width_;
    auto mipMapLevels = generateMipMaps ? static_cast<int>(std::floor(std::log2(std::max(width, height)))) + 1 : 1;
    AllocateTexture(internalFormat, mipMapLevels, width, height, format, type, std::move(pixels));
}

void TextureCubeMap::AllocateTexture(GLuint internalFormat, int mipMapLevels, int width, int height, GLuint format, GLuint type, std::vector<const void*> pixels)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glTexStorage2D(GL_TEXTURE_CUBE_MAP, mipMapLevels, internalFormat, width, height);
    for (int i = 0; i < 6; i++)
    {
        if (pixels[i])
            glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0, width, height, format, type, pixels[i]);
    }
    if (mipMapLevels > 1 && pixels[0])
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
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
    width = faces[0]->width;
    DefineTexture(faces[0]->GetInternalFormat(), width, width, false,
        { &faces[0]->data[0], &faces[1]->data[0], &faces[2]->data[0], &faces[3]->data[0], &faces[4]->data[0], &faces[5]->data[0] },
        faces[0]->GetFormat(), GL_UNSIGNED_BYTE);
    
    if (deleteAfterLoad)
        for (int i = 0; i < 6; i++)
        {
            faces[i]->data.clear(); faces[i]->data.shrink_to_fit();
        }
}

std::vector<glm::mat4> TextureCubeMap::GenerateCameraViewsForCube(glm::vec3 position)
{
    return std::vector<glm::mat4>
    {
       glm::lookAt(position, position + glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(position, position + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(position, position + glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(position, position + glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(position, position + glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(position, position + glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
}

void TextureCubeMap::RenderIntoHDRCubeMapFromTexture(int width, ShaderFactory::Type shaderType, RenderingPipeline::TextureSlot textureSourceSlot, GLuint textureSourceType, GLuint textureSource,
    bool generateMipMaps, bool renderMipMaps, int renderMipLevels)
{
    GenerateTexture(generateMipMaps);
    DefineTexture(GL_RGB16F, width, width, generateMipMaps);

    Framebuffer framebuffer(width, width);

    auto shader = Engine::GetShaderFactory().GetShader(shaderType);
    shader->setup();

    RenderingPipeline::ActivateTexture(textureSourceSlot);
    glBindTexture(textureSourceType, textureSource);

    framebuffer.Bind();
    if(renderMipMaps)
        RenderViewsIntoCubeMapWithMipMaps(shader, framebuffer, renderMipLevels, width);
    else
        RenderViewsIntoCubeMap(shader, framebuffer);
    framebuffer.Unbind();

    glBindTexture(textureSourceType, 0);
}

void TextureCubeMap::ConvertFromHDRToSRGB()
{
    auto hdrTexture = texture;
    GenerateTexture();
    DefineTexture(GL_RGB8, width, width);

    Framebuffer framebuffer(width, width);

    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::SkyboxToneMapping);
    shader->setup();

    RenderingPipeline::ActivateTexture(RenderingPipeline::TextureSlot::Environmental);
    glBindTexture(GL_TEXTURE_CUBE_MAP, hdrTexture);

    framebuffer.Bind();
    RenderViewsIntoCubeMap(shader, framebuffer);
    framebuffer.Unbind();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDeleteTextures(1, &hdrTexture);
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

        cube.Draw(shader);
    }
}

void TextureCubeMap::RenderViewsIntoCubeMapWithMipMaps(Shader* shader, Framebuffer& framebuffer, int mipLevels, int width)
{
    SimpleMesh cube(SimpleMesh::Shape::Cube);

    Camera::CameraBlock camera;
    camera.projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    auto const captureViews = GenerateCameraViewsForCube();

    for (int mipLevel = 0; mipLevel < mipLevels; mipLevel++)
    {
        int mipWidth = static_cast<int>(width * std::pow(0.5f, mipLevel));
        framebuffer.ChangeSize(mipWidth, mipWidth);
        framebuffer.Bind();

        float mipLevelUniform = static_cast<float>(mipLevel) / static_cast<float>(mipLevels - 1);
		shader->SetUniformFloat("roughness", mipLevelUniform);

        for (unsigned int i = 0; i < 6; i++)
        {
            camera.view = captureViews[i];
            RenderingPipeline::BindCameraUniform();
            glBufferData(GL_UNIFORM_BUFFER, sizeof(camera), &camera, GL_DYNAMIC_DRAW);

            framebuffer.AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, mipLevel);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            cube.Draw(shader);
        }
    }
}
