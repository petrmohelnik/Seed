#include "Mesh.h"

Mesh::Mesh()
{
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

int Mesh::NumberOfSubmeshes()
{
    return static_cast<int>(subMeshes.size());
}
