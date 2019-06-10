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
        return;

    GLuint equirectangularTexture;
    //TODO vytvorit texturu

    GLuint vao, vbo[2];
    LoadCubeMesh(&vao, &vbo[0]);

    GenerateOpenglTexture();
    DefineOpenglTexture(GL_RGB16F, 512, 512, GL_RGB, GL_FLOAT);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    GLuint captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    Camera::CameraBlock camera;
    camera.projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::EquirectangularToCubemap);
    shader->setup();

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, equirectangularTexture);

    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; ++i)
    {
        camera.view = captureViews[i];
        RenderingPipeline::BindCameraUniform();
        glBufferData(GL_UNIFORM_BUFFER, sizeof(camera), &camera, GL_DYNAMIC_DRAW);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(vao);
        shader->draw(36);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteBuffers(2, &vbo[0]);
    glDeleteVertexArrays(1, &vao);
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
        glm::uvec3(6, 5, 1),
        glm::uvec3(1, 2, 6)
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

void TextureCubeMap::GenerateOpenglTexture()
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
            faces[i]->data.clear(); faces[i]->data.clear();
    }
}

void TextureCubeMap::DefineOpenglTexture(GLuint internalFormat, int width, int height, GLuint format, GLuint type, const void* pixels)
{
    for (int i = 0; i < 6; i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, type, pixels);
    }
}
