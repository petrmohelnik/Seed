#pragma once
#include "Component.h"

class Light final : public Component
{
public:
    enum class Type
    {
        Directional,
        Point,
        Spot
    };

    using Component::Component;

    void SetType(Type type_);

    void BindLight();

private:
    struct LightBlock
    {
        alignas(16) glm::vec3 lightPos;
        alignas(16) glm::vec3 lightColor;
        alignas(16) glm::vec3 lightAmbient;
    };

    Type type = Type::Point;
};

