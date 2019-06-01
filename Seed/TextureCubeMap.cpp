#include "TextureCubeMap.h"
#include "Texture.h"

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

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, faces[i]->GetInternalFormat(), faces[i]->width, faces[i]->height, 0, faces[i]->GetFormat(), GL_UNSIGNED_BYTE, &faces[i]->data[0]);
        if(deleteAfterLoad)
            faces[i]->data.clear(); faces[i]->data.clear();
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
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
