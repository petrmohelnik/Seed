#include "Identifiable.h"

long Identifiable::uniqueIdGenerator = 0;

Identifiable::Identifiable()
{
    uniqueId = uniqueIdGenerator++;
}

Identifiable::Identifiable(const Identifiable& identifiable)
    : name(identifiable.name)
{
    uniqueId = uniqueIdGenerator++;
}

void Identifiable::SetName(std::string name_)
{
    name = std::move(name_);
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

long Identifiable::GetUniqueId()
{
    return uniqueId;
}
