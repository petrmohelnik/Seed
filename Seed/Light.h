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
	void SetColor(glm::vec3 color);
	void SetAmbientColor(glm::vec3 color);

    void BindLight();

private:
    struct LightBlock
    {
        alignas(16) glm::vec3 lightPos;
        alignas(16) glm::vec3 lightColor = glm::vec3(1.0f);
        alignas(16) glm::vec3 lightAmbient = glm::vec3(0.0f);
    };

	LightBlock dataBlock;
    Type type = Type::Point;
};

