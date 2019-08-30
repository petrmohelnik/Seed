#version 460

layout(binding = 0) uniform sampler2D texColor;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texDepth;
layout(binding = 3) uniform sampler2D texMetallic;

layout(binding = 10) uniform samplerCube texIrradiance;
layout(binding = 11) uniform samplerCube texEnvironmental;
layout(binding = 12) uniform sampler2D texBRDFIntegration;

layout(location = 0) uniform ivec2 screenSize;

in vec3 fViewPos;
in mat4 fViewProjectionInverseMatrix;

layout(location = 0) out vec4 gl_FragColor;

const float PI = 3.14159265359;

vec3 FresnelSchlickRoughness(vec3 F0, float HdotV, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - HdotV, 5.0);
}

vec3 CalculateAmbient(float NdotV, float roughness, float metallic, float ambientOclussion, vec3 F0, vec3 normal, vec3 reflectionVector, vec3 albedo)
{
	vec3 kS = FresnelSchlickRoughness(F0, NdotV, roughness);
	vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

	vec3 irradiance = texture(texIrradiance, normal).xyz;
	vec3 albedoColor = kD * irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0;
    vec3 environmentalColor = textureLod(texEnvironmental, reflectionVector, roughness * MAX_REFLECTION_LOD).xyz;
	vec2 environmentBRDF = texture(texBRDFIntegration, vec2(NdotV, roughness)).xy;
	vec3 specularColor = environmentalColor * (kS * environmentBRDF.x + environmentBRDF.y);

	return (albedoColor + specularColor) * ambientOclussion;
}

vec3 CalculateWorldPosition(vec2 texCoords)
{
    vec3 clipSpacePosition = vec3(texCoords, texture(texDepth, texCoords).r) * 2.0 - vec3(1.0);
    vec4 worldPositionHomogenous = fViewProjectionInverseMatrix * vec4(clipSpacePosition, 1.0);
    return worldPositionHomogenous.xyz / worldPositionHomogenous.w;
}

void main()
{
    vec2 texCoords = gl_FragCoord.xy / screenSize;

	vec4 colorTexture = texture(texColor, texCoords);
	vec4 normalTexture = texture(texNormal, texCoords);
	vec4 metallicTexture = texture(texMetallic, texCoords);

	vec3 albedo = colorTexture.xyz;
    bool isSpecularWorkflow = normalTexture.w != -1.0;
	float ambientOclussion = isSpecularWorkflow ? normalTexture.w : metallicTexture.x;
	float roughness = isSpecularWorkflow ? metallicTexture.w : metallicTexture.y;
	float metallic = isSpecularWorkflow ? 0.0 : metallicTexture.z;
	vec3 normal = normalize(normalTexture.xyz);

    vec3 worldPos = CalculateWorldPosition(texCoords);
	vec3 viewDir = normalize(fViewPos - worldPos);
	vec3 reflectionVector = reflect(-viewDir, normal);
	float NdotV = max(dot(normal, viewDir), 0.0);

	vec3 F0 = isSpecularWorkflow ? metallicTexture.xyz : mix(vec3(0.04), albedo, metallic);
	
	vec3 ambientColor = CalculateAmbient(NdotV, roughness, metallic, ambientOclussion, F0, normal, reflectionVector, albedo);

	gl_FragColor = vec4(ambientColor, 1.0);
}