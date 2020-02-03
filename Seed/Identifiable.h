#pragma once

class Identifiable
{
public:
    Identifiable();
    virtual ~Identifiable() = default;

    void SetName(std::string name_);
    std::string GetName();
    void AddTag(std::string tag);
    bool ContainsTag(const std::string& tag);
    long GetUniqueId();

protected:
    std::string name;
    std::unordered_set<std::string> tags;

    long uniqueId;
    static long uniqueIdGenerator;
};

