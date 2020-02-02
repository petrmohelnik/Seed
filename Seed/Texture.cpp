#include "Texture.h"
#include "Engine.h"
#include "SimpleMesh.h"
#include "Framebuffer.h"

Texture::Texture()
{
    SetColor(glm::vec4(0.0f));
}

Texture::Texture(glm::vec4 color)
{
    SetColor(color);
}

Texture::~Texture()
{
    Unload();
}

std::shared_ptr<Texture> Texture::Clone()
{
    auto clonedTexture = std::make_shared<Texture>();

    clonedTexture->data = data;
    clonedTexture->width = width;
    clonedTexture->height = height;
    clonedTexture->bytesPerPixel = bytesPerPixel;
    clonedTexture->deleteAfterLoad = deleteAfterLoad;
    clonedTexture->isSRGB = isSRGB;
    clonedTexture->isRGB = isRGB;

    if (texture != 0)
    {
        clonedTexture->GenerateTexture(GL_REPEAT, GetInternalFormat(), width, height, true);
        glCopyImageSubData(texture, GL_TEXTURE_2D, 0, 0, 0, 0,
            clonedTexture->texture, GL_TEXTURE_2D, 0, 0, 0, 0,
            width, height, 1);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    return clonedTexture;
}

void Texture::Load()
{
    if (texture != 0)
        return;

    GenerateTexture(GL_REPEAT, GetInternalFormat(), width, height, true, &data[0], GetFormat(), GL_UNSIGNED_BYTE);

    if (deleteAfterLoad)
    {
        data.clear(); data.shrink_to_fit();
    }
}

GLuint Texture::GetInternalFormat()
{
    if (bytesPerPixel == 1)
        return GL_R8;
    if (bytesPerPixel == 2)
        return GL_R16;
    if (bytesPerPixel == 3)
        return isSRGB ? GL_SRGB8 : GL_RGB8;

    return isSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8;
}

GLuint Texture::GetFormat()
{
    if (bytesPerPixel == 1)
        return GL_RED;
    if (bytesPerPixel == 2)
        return GL_RED;
    if (bytesPerPixel == 3)
        return isRGB ? GL_RGB : GL_BGR;

    return isRGB ? GL_RGBA : GL_BGRA;
}

void Texture::Unload()
{
    if (texture == 0)
        return;

    glDeleteTextures(1, &texture);
    texture = 0;
}

void Texture::Bind()
{
    Load();
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::SetColor(glm::vec4 color)
{
    data = std::vector<Uint8>{ static_cast<Uint8>(color.b * 255), static_cast<Uint8>(color.g * 255), static_cast<Uint8>(color.r * 255), static_cast<Uint8>(color.a * 255) };
    width = 1;
    height = 1;
    bytesPerPixel = 4;
    Unload();
}

void Texture::SetColor(glm::vec3 color)
{
    data = std::vector<Uint8>{ static_cast<Uint8>(color.b * 255), static_cast<Uint8>(color.g * 255), static_cast<Uint8>(color.r * 255) };
    width = 1;
    height = 1;
    bytesPerPixel = 3;
    Unload();
}

void Texture::SetColor(glm::vec2 color)
{
    data = std::vector<Uint8>{ static_cast<Uint8>(color.x * 255), static_cast<Uint8>(color.y * 255) };
    width = 1;
    height = 1;
    bytesPerPixel = 2;
    Unload();
}

void Texture::SetColor(float color)
{
    data = std::vector<Uint8>{ static_cast<Uint8>(color * 255) };
    width = 1;
    height = 1;
    bytesPerPixel = 1;
    Unload();
}

void Texture::SetIsSRGB()
{
    isSRGB = true;
}

void Texture::SetIsRGBOrder()
{
    isRGB = true;
}

void Texture::SetAlphaColor(float alpha)
{
    if (bytesPerPixel != 4 && data.size() != 4)
        throw std::runtime_error("cannot set alpha value, because data is not of size 4");

    data[3] = static_cast<int>(alpha * 255);
    Unload();
}

void Texture::AddChannel(float value)
{
    if (bytesPerPixel == 4)
        throw std::runtime_error("cannot add channel, texture already has 4");

    std::vector<Uint8> newData;
    newData.resize(data.size() / bytesPerPixel * (bytesPerPixel + 1), static_cast<int>(value * 255));

    for (int i = 0, j = 0; i < data.size(); i++, j++)
    {
        newData[j] = data[i];
        if ((i + 1) % bytesPerPixel == 0)
            j++;
    }

    data = std::move(newData);
    bytesPerPixel++;
    Unload();
}

void Texture::AddChannelFromTexture(std::shared_ptr<Texture> textureFrom, int channelFrom)
{
    if (bytesPerPixel == 4)
        throw std::runtime_error("cannot add channel, texture already has 4");
    if (textureFrom->bytesPerPixel < channelFrom)
        throw std::runtime_error("source texture does not have enough channels");
    if(width != textureFrom->width || height != textureFrom->height)
        throw std::runtime_error("source texture does not have same size as destination");

    std::vector<Uint8> newData;
    newData.resize(data.size() / bytesPerPixel * (bytesPerPixel + 1));

    for (int i = 0, j = 0, k = 0; i < data.size(); i++, j++)
    {
        newData[j] = data[i];
        if ((i + 1) % bytesPerPixel == 0)
        {
            j++;
            newData[j] = textureFrom->data[k * textureFrom->bytesPerPixel + channelFrom];
            k++;
        }
    }

    data = std::move(newData);
    bytesPerPixel++;
    Unload();
}

void Texture::GenerateTexture(GLuint wrapParam, GLuint internalFormat, int width, int height, bool generateMipMaps, const void* pixels, GLuint format, GLuint type)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMipMaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapParam);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapParam);

	auto mipMapLevels = generateMipMaps ? static_cast<int>(std::floor(std::log2(std::max(width, height)))) + 1 : 1;
    AllocateTexture(internalFormat, mipMapLevels, width, height, format, type, pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::AllocateTexture(GLuint internalFormat, int mipMapLevels, int width, int height, GLuint format, GLuint type, const void* pixels)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexStorage2D(GL_TEXTURE_2D, mipMapLevels, internalFormat, width, height);
    if (pixels)
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, pixels);
        if (mipMapLevels > 1)
            glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Texture::RenderIntoHDRTexture(int width, int height, ShaderFactory::Type shaderType, GLuint format)
{
    GenerateTexture(GL_CLAMP_TO_EDGE, GL_RG16F, width, height);

    SimpleMesh quad(SimpleMesh::Shape::Quad);

    Framebuffer framebuffer(width, height);

    auto shader = Engine::GetShaderFactory().GetShader(shaderType);
    shader->setup();

    framebuffer.Bind();
    framebuffer.AttachTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    quad.Draw(shader);
        
    framebuffer.Unbind();

    glBindTexture(GL_TEXTURE_2D, 0);
}
