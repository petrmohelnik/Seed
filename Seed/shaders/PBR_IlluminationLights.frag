#version 460

layout(std140, binding = 1) uniform LightBlock
{
	vec3 Pos;
	vec3 Color;
	vec3 Orientation;
	float Range;
	float Intensity;
	float SpotAngle;
} Light;

layout(binding = 0) uniform sampler2D texColor;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texDepth;
layout(binding = 3) uniform sampler2D texMetallic;

in vec3 fViewPos;
in vec2 fTexCoords;
in mat4 fViewProjectionInverseMatrix;

layout(location = 0) out vec4 gl_FragColor;

const float PI = 3.14159265359;

vec3 FresnelSchlick(vec3 F0, float HdotV)
{
	return F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);
}

vec3 CalculateRadiance(vec3 lightDir, vec3 worldPos)
{
	float dist = length(Light.Pos - worldPos);
	float normalizedDist = dist / Light.Range;
	float attenuation = 1.0 / (10.0 * normalizedDist * normalizedDist);
	float centerAngle = dot(lightDir, normalize(-Light.Orientation));
	float angleDifference = centerAngle - Light.SpotAngle;
	float spotAttenuation = angleDifference >= 0.0 ? 1.0 : clamp(1.0 / (1000.0 * angleDifference * angleDifference), 0.0, 1.0);
	return Light.Color * Light.Intensity * spotAttenuation * attenuation;
}

float DistributionGGX(float NdotH, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH2 = NdotH * NdotH;
	
    float denominator = (NdotH2 * (a2 - 1.0) + 1.0);
    denominator = PI * denominator * denominator;
	
    return a2 / denominator;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float denominator = NdotV * (1.0 - k) + k;
	
    return NdotV / denominator;
}

float GeometrySmith(float NdotV, float NdotL, float roughness)
{
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	
    return ggx1 * ggx2;
}

vec3 CookTorranceLobe(float NdotV, float NdotL, float NdotH, float roughness, vec3 kS)
{
	float NDF = DistributionGGX(NdotH, roughness);
	float G = GeometrySmith(NdotV, NdotL, roughness);

	vec3 numerator = NDF * G * kS;
	float denominator = 4.0 * NdotV * NdotL;
	return numerator / max(denominator, 0.0001);
}

vec3 CalculateWorldPosition()
{
    vec3 clipSpacePosition = vec3(fTexCoords, texture(texDepth, fTexCoords).r) * 2.0 - vec3(1.0);
    vec4 worldPositionHomogenous = fViewProjectionInverseMatrix * vec4(clipSpacePosition, 1.0);
    return worldPositionHomogenous.xyz / worldPositionHomogenous.w;
}

void main()
{
	vec4 colorTexture = texture(texColor, fTexCoords);
	vec4 normalTexture = texture(texNormal, fTexCoords);
	vec4 metallicTexture = texture(texMetallic, fTexCoords);

	vec3 albedo = colorTexture.xyz;
    bool isSpecularWorkflow = normalTexture.w != -1.0;
	float roughness = isSpecularWorkflow ? metallicTexture.w : metallicTexture.y;
	float metallic = isSpecularWorkflow ? 0.0 : metallicTexture.z;
	vec3 normal = normalize(normalTexture.xyz);

    vec3 worldPos = CalculateWorldPosition();
	vec3 viewDir = normalize(fViewPos - worldPos);
	vec3 lightDir = normalize(Light.Pos - worldPos);
	vec3 halfVector = normalize(lightDir + viewDir);
	float NdotV = max(dot(normal, viewDir), 0.0);
	float NdotL = max(dot(normal, lightDir), 0.0);
	float NdotH = max(dot(normal, halfVector), 0.0);
	float HdotV = max(dot(halfVector, viewDir), 0.0);

	vec3 F0 = isSpecularWorkflow ? metallicTexture.xyz : mix(vec3(0.04), albedo, metallic);
	vec3 kS = FresnelSchlick(F0, HdotV);
	vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
	
	vec3 radiance = CalculateRadiance(lightDir, worldPos);
	vec3 diffuseColor = kD * albedo / PI * radiance * NdotL;
	vec3 specularColor = CookTorranceLobe(NdotV, NdotL, NdotH, roughness, kS) * radiance * NdotL;

	gl_FragColor = vec4(diffuseColor + specularColor, 1.0);
}