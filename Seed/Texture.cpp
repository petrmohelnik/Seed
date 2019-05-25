#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
{
    SetColor(glm::vec4(1.0f));
}

Texture::~Texture()
{
    Unload();
}

void Texture::Load()
{
    if (texture != 0)
        return;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GetInternalFormat(), width, height, 0, GetFormat(), GL_UNSIGNED_BYTE, &data[0]);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glBindTexture(GL_TEXTURE_2D, 0);

    if (deleteAfterLoad)
    {
        std::vector<Uint8> emptyVector;
        data.swap(emptyVector);
    }
}

GLuint Texture::GetInternalFormat()
{
	if (bytesPerPixel == 1)
		return GL_R8;
	if (bytesPerPixel == 2)
		return GL_R16;
	if (bytesPerPixel == 3)
		return GL_RGB8;

	return GL_RGBA8;
}

GLuint Texture::GetFormat()
{
	if (bytesPerPixel == 1)
		return GL_RED;
	if (bytesPerPixel == 2)
		return GL_RED;
    if (bytesPerPixel == 3)
        return GL_BGR;

	return GL_BGRA;
}

void Texture::Unload()
{
    if (texture == 0)
        return;

    glDeleteTextures(1, &texture);
    texture = 0;
}

void Texture::BindTexture()
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
