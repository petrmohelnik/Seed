#pragma once
#include "Component.h"

class Camera;

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
    float GetIntensity();
    void SetSpotAngle(float outerAngle, float blend);
    void SetDirection(glm::vec3 direction);
    void SetDirectionalLight(glm::vec3 direction);
    void SetPointLight(float range, float intensity);
    void SetSpotLight(glm::vec3 direction, float range, float intensity, float spotOuterAngle, float spotBlend);
    void SetSizeUV(float size);
    void SetShadowNearPlaneDistance(float distance);
    void SetShadowFarPlaneDistance(float distance);
    void SetIsShadowCaster(bool castShadows);

    void BindLight(const Camera& camera);
    bool IsVisible(const Camera& camera);

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
        IsShadowCaster = 0x00000008u
    };

    struct LightBlock
    {
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        alignas(16) glm::vec3 Pos;
        alignas(16) glm::vec3 Color = glm::vec3(1.0f);
        alignas(16) glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, 1.0f);
        alignas(4) float Range = 0.0f;
        alignas(4) float Intensity = 1.0f;
        alignas(4) float SizeUV = 0.2f;
        alignas(4) float SpotAngle = 1.57f;
        alignas(4) float SpotAngleScale = 0.0f;
        alignas(4) float SpotAngleOffset = 1.0f;
        alignas(4) float ShadowFarPlane = 1.0f;
        alignas(4) float ViewPortScale = 1.0f;
        alignas(4) unsigned int Type = LightTypeMask::Directional;
    };

    void SetSpotAngleToDataBlock();
    void SetLightTypeToDataBlock();
    void CalculateOptimumViewPortScale(const Camera& camera);
    void SetupShadowData(const Camera& camera);

    LightBlock dataBlock;
    Type type = Type::Point;
    float spotOuterAngle = 1.57f;
    float spotBlend = 0.1f;
    float shadowNearPlane = 0.1f;
    float shadowFarPlane = 1000.0f;
    bool isShadowCaster = true;
};
