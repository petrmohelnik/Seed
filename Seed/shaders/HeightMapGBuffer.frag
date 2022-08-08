#version 460

layout(std140, binding = 3) uniform MaterialBlock
{
    bool SpecularWorkflow;
    bool UseOcclusionMap;
    float ParallaxStrength;
} Material;

layout(binding = 0) uniform sampler2D texAlbedo;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texHeight;
layout(binding = 3) uniform sampler2D texMetallic;
layout(binding = 4) uniform sampler2D texEmission;
layout(binding = 5) uniform sampler2D texOcclusion;

in vec3 fPos;
in vec3 fViewPos;
in vec2 fTexCoords;
in mat3 fTBN;

layout (location = 0) out vec4 gColor;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gMetallic;
layout (location = 3) out vec3 gIllumination;

void main()
{
    vec3 viewDir = normalize(fViewPos - fPos);
    vec2 texCoords = fTexCoords;

    vec4 albedoTexture = texture(texAlbedo, texCoords);
    vec3 normalTexture = texture(texNormal, texCoords).xyz;
    vec4 metallicTexture = texture(texMetallic, texCoords);
    vec3 emissionTexture = texture(texEmission, texCoords).xyz;
    float occlusionTexture = texture(texOcclusion, texCoords).x;

    float ambientOclussion = (Material.SpecularWorkflow || Material.UseOcclusionMap) ? occlusionTexture : metallicTexture.x;
    float roughness = Material.SpecularWorkflow ? 1.0 - metallicTexture.w : metallicTexture.y;
    float metallic = Material.SpecularWorkflow ? 0.0 : metallicTexture.z;
    vec3 normal = normalize(normalTexture * 2.0 - 1.0); normal.y = -normal.y;
    normal = normalize(fTBN * normal);

    float alpha = albedoTexture.w;
    if(alpha < 0.1)
        discard;

    gColor = albedoTexture;
    //gColor(vec3(texture(texHeight, texCoords).r), 1.0)
    if(Material.SpecularWorkflow)
    {
        gNormal = vec4(normal, ambientOclussion);
        gMetallic = vec4(metallicTexture.xyz, roughness);
    }
    else
    {
        gNormal = vec4(normal, -1.0);
        gMetallic = vec4(ambientOclussion, roughness, metallic, 0.0);
    }
    gIllumination = emissionTexture;
}