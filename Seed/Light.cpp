#include "Light.h"
#include "RenderingPipeline.h"
#include "Transform.h"
#include "Engine.h"
#include "Camera.h"

void Light::SetType(Type type_)
{
    type = type_;
}

void Light::SetColor(glm::vec3 color)
{
    dataBlock.Color = color;
}

void Light::SetRange(float range)
{
    dataBlock.Range = range;
}

void Light::SetIntensity(float intensity)
{
    dataBlock.Intensity = intensity;
}

float Light::GetIntensity()
{
    return dataBlock.Intensity;
}

void Light::SetSpotAngle(float outerAngle, float blend)
{
    spotOuterAngle = outerAngle;
    spotBlend = blend;

    SetSpotAngleToDataBlock();
}

void Light::SetDirection(glm::vec3 direction)
{
    GetTransform()->LookAt(GetTransform()->GetPosition() + direction);
}

void Light::SetDirectionalLight(glm::vec3 direction)
{
    SetType(Type::Directional);
    SetDirection(direction);
}

void Light::SetPointLight(float range, float intensity)
{
    SetType(Type::Point);
    SetRange(range);
    SetIntensity(intensity);
}

void Light::SetSpotLight(glm::vec3 direction, float range, float intensity, float spotOuterAngle, float spotBlend)
{
    SetType(Type::Spot);
    SetDirection(direction);
    SetRange(range);
    SetSpotAngle(spotOuterAngle, spotBlend);
    SetIntensity(intensity);
}

void Light::SetSizeUV(float size)
{
    dataBlock.SizeUV = size;
}

void Light::SetShadowNearPlaneDistance(float distance)
{
    shadowNearPlane = distance;
}

void Light::SetShadowFarPlaneDistance(float distance)
{
    shadowFarPlane = distance;
}

void Light::SetIsShadowCaster(bool castShadows)
{
    isShadowCaster = castShadows;
}

void Light::SetSpotAngleToDataBlock()
{
    float outerAngle = spotOuterAngle * 0.5f;
    float innerAngle = outerAngle * (1.0f - spotBlend);
    dataBlock.SpotAngleScale = 1.0f / std::max(0.001f, (std::cos(innerAngle) - std::cos(outerAngle)));
    dataBlock.SpotAngleOffset = -glm::cos(outerAngle) * dataBlock.SpotAngleScale;
}

static unsigned int NextPowerOfTwo(glm::uint32 v)
{
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;

    return v;
}

void Light::CalculateOptimumViewPortScale(const Camera& camera)
{
    auto maxShadowMapSize = type == Light::Type::Spot ? RenderingPipeline::MaxSpotLightShadowMapSize : RenderingPipeline::MaxPointLightShadowMapSize;
    auto lightToCameraDist = glm::length(GetTransform()->GetPosition() - camera.GetTransform()->GetPosition());
    auto screenPosition = camera.dataBlock.projection * glm::vec4(dataBlock.ShadowFarPlane, 0.0f, -lightToCameraDist, 1.0f);
    auto pixelSize = static_cast<glm::uint32>(screenPosition.x / screenPosition.w * Engine::GetWindow().GetWindowSize().x * 0.5f);

    auto shadowMapSize = glm::clamp(static_cast<int>(NextPowerOfTwo(pixelSize)), 0, maxShadowMapSize);
    dataBlock.ViewPortScale = static_cast<float>(shadowMapSize) / maxShadowMapSize;
}

void Light::SetupShadowData(const Camera & camera)
{
    dataBlock.ShadowFarPlane = dataBlock.Range == 0.0f ? shadowFarPlane : std::min(dataBlock.Range, shadowFarPlane);
    if (type == Type::Spot)
    {
        dataBlock.ViewMatrix = glm::lookAt(
            dataBlock.Pos,
            dataBlock.Pos + dataBlock.Orientation,
            glm::vec3(0.0f, 1.0f, 0.0f));

        dataBlock.ProjectionMatrix = glm::perspective(spotOuterAngle, 1.0f, shadowNearPlane, dataBlock.ShadowFarPlane);
        dataBlock.SpotAngle = spotOuterAngle;
    }
    else if (type == Type::Point)
    {
        dataBlock.ProjectionMatrix = glm::perspective(glm::radians(90.0f), 1.0f, shadowNearPlane, dataBlock.ShadowFarPlane);
    }

    CalculateOptimumViewPortScale(camera);
}

void Light::SetLightTypeToDataBlock()
{
    dataBlock.Type = 0;
    if (type == Type::Directional)
        dataBlock.Type = LightTypeMask::Directional;
    if (type == Type::Point)
        dataBlock.Type = LightTypeMask::Point;
    if (type == Type::Spot)
        dataBlock.Type = LightTypeMask::Spot;

    if (isShadowCaster)
        dataBlock.Type |= LightTypeMask::IsShadowCaster;
}

void Light::BindLight(const Camera& camera)
{
    RenderingPipeline::BindLightUniform();

    SetLightTypeToDataBlock();
    dataBlock.Pos = GetTransform()->GetPosition();
    dataBlock.Orientation = glm::normalize(glm::mat3(glm::inverse(glm::transpose(GetTransform()->GetModelMatrix()))) * glm::vec3(0.0f, 0.0f, -1.0f));

    dataBlock.ViewMatrix == glm::mat4(1.0f);
    dataBlock.ProjectionMatrix == glm::mat4(1.0f);
    
    if (isShadowCaster)
        SetupShadowData(camera);

    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);
}

bool Light::IsVisible(const Camera& camera)
{
    if (type == Type::Directional)
        return true;

    if (type == Type::Spot)
    {
        Frustum lightFrustum;
        lightFrustum.Update(dataBlock.ProjectionMatrix * dataBlock.ViewMatrix);

        glm::vec3 min(std::numeric_limits<float>::max());
        glm::vec3 max(std::numeric_limits<float>::lowest());
        for (int i = 0; i < 8; i++)
        {
            auto point = lightFrustum.GetPoint(i);
            if (point.x < min.x) min.x = point.x;
            if (point.y < min.y) min.y = point.y;
            if (point.z < min.z) min.z = point.z;
            if (point.x > max.x) max.x = point.x;
            if (point.y > max.y) max.y = point.y;
            if (point.z > max.z) max.z = point.z;
        }
        AABB aabb(min, max);

        return aabb.TestFrustum(camera.GetFrustum());
    }

    if (type == Type::Point)
    {
        AABB aabb(dataBlock.Pos - dataBlock.ShadowFarPlane, dataBlock.Pos + dataBlock.ShadowFarPlane);
        return aabb.TestFrustum(camera.GetFrustum());
    }

    return false;
}

void Light::OnInputGraphUpdate()
{
    Engine::GetInput().BulletText("Light");
    Engine::GetInput().SliderFloatLog("Range", dataBlock.Range, 0.0, 100);
    Engine::GetInput().SliderFloatLog("Intensity", dataBlock.Intensity, 0.0, 100);
    Engine::GetInput().SliderFloat("SizeUV", dataBlock.SizeUV, 0.0, 1.0);
}
