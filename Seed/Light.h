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
    void SetSpotAngle(float outerAngle, float blend);
    void SetDirection(glm::vec3 direction);
    void SetDirectionalLight(glm::vec3 direction);
    void SetPointLight(float range, float intensity);
    void SetSpotLight(glm::vec3 direction, float range, float intensity, float spotOuterAngle, float spotBlend);
    void SetShadowNearPlaneDistance(float distance);
    void SetShadowFarPlaneDistance(float distance);
    void SetShadowCaster(bool castShadows);

    void BindLight();

protected:
    friend class FileSystem;
    friend class RenderingPipeline;
    void OnInputGraphUpdate() override;

private:
    enum LightTypeMask
    {
        Directional = 0x00000001u,
        Point = 0x00000002u,
        Spot = 0x00000004u,
        CastShadow = 0x00000008u
    };

    struct LightBlock
    {
        glm::mat4 LightSpaceMatrix;
        alignas(16) glm::vec3 Pos;
        alignas(16) glm::vec3 Color = glm::vec3(1.0f);
        alignas(16) glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, 1.0f);
        alignas(4) float Range = 0.0f;
        alignas(4) float Itensity = 1.0f;
        alignas(4) float SpotAngleScale = 0.0f;
        alignas(4) float SpotAngleOffset = 1.0f;
        alignas(4) unsigned int Type = LightTypeMask::Directional;
    };

    void SetSpotAngleToDataBlock();
    void SetLighTypeToDataBlock();

	LightBlock dataBlock;
    Type type = Type::Point;
    float spotOuterAngle = 1.57f;
    float spotBlend = 0.1f;
    float shadowNearPlaneDistance = 0.1f;
    float shadowFarPlaneDistance = 1000.0f;
    bool shadowCaster = true;
};
