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
        glm::vec4 lightPos;
        glm::vec4 lightColor;
        glm::vec4 lightAmbient;
    };

    Type type = Type::Point;
};

