#include "Light.h"
#include "RenderingPipeline.h"
#include "Transform.h"
#include "Engine.h"

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
    dataBlock.Itensity = intensity;
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

void Light::SetShadowNearPlaneDistance(float distance)
{
    shadowNearPlaneDistance = distance;
}

void Light::SetShadowFarPlaneDistance(float distance)
{
    shadowFarPlaneDistance = distance;
}

void Light::SetShadowCaster(bool castShadows)
{
    shadowCaster = castShadows;
}

void Light::SetSpotAngleToDataBlock()
{
    float outerAngle = spotOuterAngle * 0.5f;
    float innerAngle = outerAngle * (1.0f - spotBlend);
    dataBlock.SpotAngleScale = 1.0f / std::max(0.001f, (std::cos(innerAngle) - std::cos(outerAngle)));
    dataBlock.SpotAngleOffset = -glm::cos(outerAngle) * dataBlock.SpotAngleScale;
}

void Light::SetLighTypeToDataBlock()
{
    dataBlock.Type = 0;
    if (type == Type::Directional)
        dataBlock.Type = LightTypeMask::Directional;
    if (type == Type::Point)
        dataBlock.Type = LightTypeMask::Point;
    if (type == Type::Spot)
        dataBlock.Type = LightTypeMask::Spot;

    if (shadowCaster)
        dataBlock.Type |= LightTypeMask::CastShadow;
}

void Light::BindLight()
{
    RenderingPipeline::BindLightUniform();

    SetLighTypeToDataBlock();
	dataBlock.Pos = GetTransform()->GetPosition();
    dataBlock.Orientation = glm::normalize(glm::mat3(glm::inverse(glm::transpose(GetTransform()->GetModelMatrix()))) * glm::vec3(0.0f, 0.0f, -1.0f));

    dataBlock.LightSpaceMatrix == glm::mat4(1.0f);
    if (shadowCaster && type == Type::Spot)
    {
        glm::mat4 lightProjection = glm::perspective(spotOuterAngle, 1.0f, shadowNearPlaneDistance, 
            dataBlock.Range == 0.0f ? shadowFarPlaneDistance : std::min(dataBlock.Range, shadowFarPlaneDistance));
        glm::mat4 lightView = glm::lookAt(
            dataBlock.Pos,
            dataBlock.Pos + dataBlock.Orientation,
            glm::vec3(0.0f, 1.0f, 0.0f));
        
        dataBlock.LightSpaceMatrix = lightProjection * lightView;
    }

    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);
}

void Light::OnInputGraphUpdate()
{
    ImGui::BulletText("Light");
    Engine::GetInput().SliderFloatLog("Range", dataBlock.Range, 0.0, 100);
    Engine::GetInput().SliderFloatLog("Intensity", dataBlock.Itensity, 0.0, 100);
}
