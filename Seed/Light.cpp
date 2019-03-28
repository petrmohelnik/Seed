#include "Light.h"
#include "RenderingPipeline.h"

void Light::SetType(Type type_)
{
    type = type_;
}

void Light::BindLight()
{
    RenderingPipeline::BindLightUniform();
    LightBlock lightBlockData{ glm::vec4(1000.0f), glm::vec4(1.0f), glm::vec4(0.2f) };
    glBufferData(GL_UNIFORM_BUFFER, sizeof(lightBlockData), &lightBlockData, GL_DYNAMIC_DRAW);
}
