#pragma once

class Identifiable
{
public:
    Identifiable();
    Identifiable(const Identifiable& identifiable);
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

