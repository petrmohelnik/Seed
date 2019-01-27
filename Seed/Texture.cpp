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
}

void Texture::Unload()
{
}

void Texture::SetColor(glm::u8vec4 color)
{
    data = std::vector<Uint8>{ color.x, color.y, color.z, color.w };
    width = 1;
    height = 1;
}
