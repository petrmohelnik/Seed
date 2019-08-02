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
    outerAngle *= 0.5f;
    float innerAngle = outerAngle * (1.0f - blend);
    dataBlock.SpotAngleScale = 1.0f / std::max(0.001f, (std::cos(innerAngle) - std::cos(outerAngle)));
    dataBlock.SpotAngleOffset = -glm::cos(outerAngle) * dataBlock.SpotAngleScale;
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

void Light::SetPointLight(glm::vec3 position, float range, float intensity)
{
    SetType(Type::Point);
    SetRange(range);
    SetIntensity(intensity);
    dataBlock.SpotAngleScale = 0.0f;
    dataBlock.SpotAngleOffset = 1.0f;
}

void Light::SetSpotLight(glm::vec3 position, glm::vec3 direction, float range, float intensity, float spotOuterAngle, float spotBlend)
{
    SetType(Type::Spot);
    SetDirection(direction);
    SetRange(range);
    SetSpotAngle(spotOuterAngle, spotBlend);
    SetIntensity(intensity);
}

void Light::BindLight()
{
    RenderingPipeline::BindLightUniform();

	dataBlock.Pos = GetTransform()->GetPosition();
    dataBlock.Orientation = glm::normalize(glm::mat3(glm::inverse(glm::transpose(GetTransform()->GetModelMatrix()))) * glm::vec3(0.0f, 0.0f, -1.0f));
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);
}

void Light::OnInputGraphUpdate()
{
    ImGui::BulletText("Light");
    Engine::GetInput().SliderFloatLog("Range", dataBlock.Range, -0.0, 100);
    Engine::GetInput().SliderFloatLog("Intensity", dataBlock.Itensity, 0.0, 100);
}
