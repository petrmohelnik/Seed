#include "Light.h"
#include "RenderingPipeline.h"
#include "Transform.h"

void Light::SetType(Type type_)
{
    type = type_;
}

void Light::SetColor(glm::vec3 color)
{
	dataBlock.lightColor = color;
}

void Light::SetAmbientColor(glm::vec3 color)
{
	dataBlock.lightAmbient = color;
}

void Light::SetRange(float range)
{
    dataBlock.lightAttenuationCoeffs.x = 1.0f;
    dataBlock.lightAttenuationCoeffs.y = 4.5f / range;
    dataBlock.lightAttenuationCoeffs.z = 75.0f / (range * range);
}

void Light::SetCutoffAngle(float angle)
{
    dataBlock.lightCutoff.x = glm::cos(angle);
    dataBlock.lightCutoff.y = glm::cos(angle * 1.25f);
}

void Light::SetDirection(glm::vec3 direction)
{
    GetTransform()->LookAtLocal(glm::normalize(direction));
}

void Light::SetDirectionalLight(glm::vec3 direction)
{
    SetType(Type::Directional);
    SetDirection(direction);
}

void Light::SetPointLight(glm::vec3 position, float range)
{
    SetType(Type::Point);
    SetRange(range);
    dataBlock.lightCutoff = glm::vec2(-1.0f, -1.0f);
}

void Light::SetSpotLight(glm::vec3 position, glm::vec3 direction, float range, float cutoffAngle)
{
    SetType(Type::Spot);
    SetDirection(direction);
    SetRange(range);
    SetCutoffAngle(cutoffAngle);
}

void Light::BindLight()
{
    RenderingPipeline::BindLightUniform();

	dataBlock.lightPos = GetTransform()->GetPosition();
    dataBlock.lightOrientation = glm::mat3(glm::inverse(glm::transpose(GetTransform()->GetModelMatrix()))) * glm::vec3(0.0f, 0.0f, -1.0f);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);
}
