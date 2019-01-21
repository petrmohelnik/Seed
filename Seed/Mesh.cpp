#include "Mesh.h"

Mesh::Mesh()
{
    vertices = std::vector<glm::vec3>();
}

Mesh::~Mesh()
{
    Unload();
}

void Mesh::Load()
{
    //load na gpu
}

void Mesh::Unload()
{
    //unload z gpu
}
