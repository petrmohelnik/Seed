#include "Identifiable.h"

void Identifiable::SetName(std::string name_)
{
    name = std::move(name);
}

std::string Identifiable::GetName()
{
    return name;
}

void Identifiable::AddTag(std::string tag)
{
    tags.emplace(std::move(tag));
}

bool Identifiable::ContainsTag(const std::string& tag)
{
    return tags.count(name) != 0;
}
