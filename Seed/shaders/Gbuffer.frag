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

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    float currentHeight = 1.0;
	vec2 currentTexCoords = texCoords;
	float currentTextureHeight = texture(texHeight, currentTexCoords).r;
	if(currentTextureHeight == 1.0)
		return texCoords;

	viewDir.y = -viewDir.y;

    const float minSteps = 32.0;
	const float maxSteps = 8.0;
	float numSteps = mix(maxSteps, minSteps, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    
    float deltaHeight = 1.0 / numSteps;
    
    vec2 P = viewDir.xy / (viewDir.z + 0.42) * Material.ParallaxStrength; 
    vec2 deltaTexCoords = P / numSteps;
  
	for(int i = 0; i < numSteps && currentHeight > currentTextureHeight; i++)
	{
		currentTexCoords -= deltaTexCoords;
		currentHeight -= deltaHeight;
		currentTextureHeight = texture(texHeight, currentTexCoords).r;
	}

	for(int i = 0; i < numSteps / 4; i++)
	{
		deltaTexCoords *= 0.5;
		deltaHeight *= 0.5; 

		if(currentHeight < currentTextureHeight)
		{
			currentTexCoords += deltaTexCoords;
			currentHeight += deltaHeight;  
		}
		else
		{
			currentTexCoords -= deltaTexCoords;
			currentHeight -= deltaHeight;
		}
		currentTextureHeight = texture(texHeight, currentTexCoords).r;
	}

	return currentTexCoords;
}

void main()
{
	vec3 viewDir = normalize(fViewPos - fPos);
	vec2 texCoords = ParallaxMapping(fTexCoords, viewDir);

	vec4 albedoTexture = texture(texAlbedo, texCoords);
	vec3 normalTexture = texture(texNormal, texCoords).xyz;
	vec4 metallicTexture = texture(texMetallic, texCoords);
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
}