#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
{
    SetColor(glm::u8vec4(255, 255, 255, 0));
}

Texture::~Texture()
{
    Unload();
}

void Texture::Load()
{
    glGenTextures(1, &sampler);
    glBindTexture(GL_TEXTURE_2D, sampler);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Unload()
{
    glDeleteTextures(1, &sampler);
}

void Texture::SetColor(glm::u8vec4 color)
{
    data = std::vector<Uint8>{ color.x, color.y, color.z, color.w };
    width = 1;
    height = 1;
}
