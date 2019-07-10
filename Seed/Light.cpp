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

void Light::SetSpotAngle(float spotAngle)
{
    dataBlock.SpotAngle = glm::cos(spotAngle);
}

void Light::SetDirection(glm::vec3 direction)
{
    direction.x += 0.001f;
    GetTransform()->LookAtLocal(direction);
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
    dataBlock.SpotAngle = -1.0f;
}

void Light::SetSpotLight(glm::vec3 position, glm::vec3 direction, float range, float spotAngle, float intensity)
{
    SetType(Type::Spot);
    SetDirection(direction);
    SetRange(range);
    SetSpotAngle(spotAngle);
    SetIntensity(intensity);
}

void Light::BindLight()
{
    Engine::GetInput().SliderFloat("Range", dataBlock.Range, 0.0, 1000);
    Engine::GetInput().SliderFloat("Intensity", dataBlock.Itensity, 0.0, 100);

    RenderingPipeline::BindLightUniform();

	dataBlock.Pos = GetTransform()->GetPosition();
    dataBlock.Orientation = glm::normalize(glm::mat3(glm::inverse(glm::transpose(GetTransform()->GetModelMatrix()))) * glm::vec3(0.0f, 0.0f, -1.0f));
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);
}
