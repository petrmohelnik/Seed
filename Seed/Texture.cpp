#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
{
    SetColor(255, 255, 255, 255);
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
    glDeleteTextures(1, &texture);
    texture = 0;
}

void Texture::BindTexture()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    data = std::vector<Uint8>{ b, g, r, a };
    width = 1;
    height = 1;
	bytesPerPixel = 4;
}

void Texture::SetColor(Uint8 r, Uint8 g, Uint8 b)
{
	data = std::vector<Uint8>{ b, g, r };
	width = 1;
	height = 1;
	bytesPerPixel = 3;
}

void Texture::SetColor(Uint8 r1, Uint8 r2)
{
	data = std::vector<Uint8>{ r1, r2 };
	width = 1;
	height = 1;
	bytesPerPixel = 2;
}

void Texture::SetColor(Uint8 r)
{
	data = std::vector<Uint8>{ r };
	width = 1;
	height = 1;
	bytesPerPixel = 1;
}
