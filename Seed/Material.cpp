#include "Material.h"

Material::Material()
{
}

Material::~Material()
{
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
