#include "Skybox.h"
#include "TextureCubeMap.h"
#include "Engine.h"

Skybox::Skybox(std::shared_ptr<TextureCubeMap> cubeMap) : cubeMap(cubeMap)
{
    cubeMap->Load();
    LoadCubeMesh();
}

void Skybox::LoadCubeMesh()
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

    glGenBuffers(2, &vbo[0]);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, &vertices[0].x, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Uint32) * 3, &indices[0].x, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Skybox::~Skybox()
{
    glDeleteBuffers(2, &vbo[0]);
    glDeleteVertexArrays(1, &vao);
}

std::shared_ptr<TextureCubeMap> Skybox::GetCubeMap()
{
    cubeMap = std::make_shared<TextureCubeMap>(*cubeMap);
    return cubeMap;
}

std::shared_ptr<TextureCubeMap> Skybox::GetSharedCubeMap()
{
    return cubeMap;
}

void Skybox::Render()
{
	glActiveTexture(GL_TEXTURE5);
    cubeMap->BindTexture();
    auto shader = Engine::GetShaderFactory().GetShader(ShaderFactory::Type::Skybox);
    shader->setup();

    glBindVertexArray(vao);

    shader->draw(36);
}

TextureCubeMap * Skybox::GetCubeMapRawPtr()
{
    return cubeMap.get();
}
