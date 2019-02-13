#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
    Unload();
}

void Material::Load()
{
    diffuse.Load();
    specular.Load();
    normal.Load();
    height.Load();
}

void Material::Unload()
{
    diffuse.Unload();
    specular.Unload();
    normal.Unload();
    height.Unload();
}
