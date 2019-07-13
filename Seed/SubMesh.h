#pragma once

class SubMesh
{
public:
	SubMesh();
	~SubMesh();

	void Load();
	void Unload();

	void Bind();

	int GetIndicesCount();

protected:
	friend class FileSystem;
	friend class Mesh;

private:
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::uvec3> indices;
	int indicesCount;
	GLuint vao = 0;
	GLuint vbo[6];

	bool deleteAfterLoad = true;
};

