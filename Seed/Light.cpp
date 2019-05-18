#include "Light.h"
#include "RenderingPipeline.h"
#include "Transform.h"

void Light::SetType(Type type_)
{
    type = type_;
}

void Light::BindLight()
{
    RenderingPipeline::BindLightUniform();
    LightBlock lightBlockData
    { 
        GetTransform()->GetPosition(),
        glm::vec3(1.0f),
        glm::vec3(0.05f) 
    };
    glBufferData(GL_UNIFORM_BUFFER, sizeof(lightBlockData), &lightBlockData, GL_DYNAMIC_DRAW);
}
