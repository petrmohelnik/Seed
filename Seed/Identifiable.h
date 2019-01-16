#pragma once

class Identifiable
{
public:
    virtual ~Identifiable() = default;

    void SetName(std::string name_);
    std::string GetName();
    void AddTag(std::string tag);
    bool ContainsTag(const std::string& tag);

protected:
    std::string name;
    std::unordered_set<std::string> tags;
};

