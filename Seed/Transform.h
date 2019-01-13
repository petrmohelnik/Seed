#pragma once
#include "Component.h"

class Transform final : public Component
{
public:
    using Component::Component;
    Transform* GetParent();
    void SetParent(Transform* parent_);
    void SetParent(Object* parent_);

protected:
    friend class Object;
    friend class Objects;
    void DestroyAllChildren();
    void CleanChildren();

private:
    void AddChild(Transform* child);

    Transform* parent = nullptr;
    std::vector<Transform*> children;

    glm::mat3 position;
    glm::mat3 rotation;
};
