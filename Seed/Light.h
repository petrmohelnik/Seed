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
    void SetRange(float range);
    void SetCutoffAngle(float angle);
    void SetDirection(glm::vec3 direction);
    void SetDirectionalLight(glm::vec3 direction);
    void SetPointLight(glm::vec3 position, float range);
    void SetSpotLight(glm::vec3 position, glm::vec3 direction, float range, float cutoffAngle);

    void BindLight();

private:
    struct LightBlock
    {
        alignas(16) glm::vec3 lightPos;
        alignas(16) glm::vec3 lightColor = glm::vec3(1.0f);
        alignas(16) glm::vec3 lightAttenuationCoeffs = glm::vec3(1.0f, 0.0f, 0.0f);
        alignas(8) glm::vec2 lightCutoff = glm::vec2(-1.0f, -1.0f);
        alignas(16) glm::vec3 lightOrientation = glm::vec3(0.0f, 0.0f, 1.0f);
        alignas(16) glm::vec3 lightAmbient = glm::vec3(0.0f);
    };

	LightBlock dataBlock;
    Type type = Type::Point;
};

