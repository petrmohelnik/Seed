#include "stdafx.h"
#include "Texture.h"
#include "Engine.h"

Texture::Texture()
{
    SetColor(glm::vec4(1.0f));
}

Texture::Texture(glm::vec4 color)
{
	SetColor(color);
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

void Texture::SetAlphaColor(float alpha)
{
    if (bytesPerPixel != 4 && data.size() != 4)
        throw std::runtime_error("cannot set alpha value, because data is not of size 4");

    data[3] = static_cast<int>(alpha * 255);
    Unload();
}

void Texture::AddAlphaChannel(float alpha)
{
    if (bytesPerPixel != 3)
        throw std::runtime_error("cannot add alpha value, because data is not of size 3");

    std::vector<Uint8> newData;
    newData.resize(data.size() / 3 * 4, static_cast<int>(alpha * 255));

    for (int i = 0, j = 0; i < data.size(); i++, j++)
    {
        newData[j] = data[i];
        if ((i + 1) % 3 == 0)
            j++;
    }

    data = std::move(newData);
    bytesPerPixel = 4;
    Unload();
}

void Texture::LoadQuadMesh(GLuint* vao, GLuint* vbo)
{
    std::vector<glm::vec3> vertices =
    {
        glm::vec3(-1.0f,  1.0f, 0.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f,  1.0f, 0.0f),
    };

    std::vector<glm::uvec3> indices =
    {
        glm::uvec3(0, 1, 2),
        glm::uvec3(2, 3, 0),
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

void Texture::UnloadQuadMesh(GLuint* vao, GLuint* vbo)
{
    glDeleteBuffers(2, vbo);
    glDeleteVertexArrays(1, vao);
}

void Texture::RenderIntoHDRTexture(float width, float height, ShaderFactory::Type shaderType, GLuint format)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, format, GL_FLOAT, nullptr);

    GLuint vao, vbo[2];
    LoadQuadMesh(&vao, &vbo[0]);

    GLuint fbo, rbo;
    Texture::GenerateFrameBuffer(&fbo, &rbo, width, width);

    auto shader = Engine::GetShaderFactory().GetShader(shaderType);
    shader->setup();

    glViewport(0, 0, width, width);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(vao);
    shader->draw(6);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Engine::GetWindow().ResetViewport();

    UnloadQuadMesh(&vao, &vbo[0]);
    Texture::DeleteFrameBuffer(&fbo, &rbo);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::GenerateFrameBuffer(GLuint* fbo, GLuint* rbo, int width, int height)
{
    glGenFramebuffers(1, fbo);
    glGenRenderbuffers(1, rbo);

    glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
    glBindRenderbuffer(GL_RENDERBUFFER, *rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, *rbo);
}

void Texture::DeleteFrameBuffer(GLuint* fbo, GLuint* rbo)
{
    glDeleteRenderbuffers(1, rbo);
    glDeleteFramebuffers(1, fbo);
}
