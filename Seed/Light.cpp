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

void Light::BindLight()
{
    RenderingPipeline::BindLightUniform();

	dataBlock.lightPos = GetTransform()->GetPosition();
    glBufferData(GL_UNIFORM_BUFFER, sizeof(dataBlock), &dataBlock, GL_DYNAMIC_DRAW);
}
