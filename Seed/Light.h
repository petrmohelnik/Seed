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
    void SetRange(float range);
    void SetIntensity(float intensity);
    void SetSpotAngle(float spotAngle);
    void SetDirection(glm::vec3 direction);
    void SetDirectionalLight(glm::vec3 direction);
    void SetPointLight(glm::vec3 position, float range, float intensity);
    void SetSpotLight(glm::vec3 position, glm::vec3 direction, float range, float spotAngle, float intensity);

    void BindLight();

protected:
    friend class FileSystem;

private:
    struct LightBlock
    {
        alignas(16) glm::vec3 Pos;
        alignas(16) glm::vec3 Color = glm::vec3(1.0f);
        alignas(16) glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, 1.0f);
        alignas(4) float Range = 1.0f;
        alignas(4) float Itensity = 1.0f;
        alignas(4) float SpotAngle = -1.0f;
    };

	LightBlock dataBlock;
    Type type = Type::Point;
};

