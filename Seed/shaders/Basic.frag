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
layout(binding = 10) uniform samplerCube texIrradiance;
layout(binding = 11) uniform samplerCube texEnvironmental;
layout(binding = 12) uniform sampler2D texBRDFIntegration;

in vec3 fPos;
in vec3 fViewPos;
in vec3 fLightPos;
in vec3 fLightOrienation;
in vec2 fTexCoords;
in mat3 fTBN;

layout(location = 0) out vec4 gl_FragColor;

const float PI = 3.14159265359;

vec3 FresnelSchlick(vec3 F0, float HdotV)
{
	return F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);
}

vec3 FresnelSchlickRoughness(vec3 F0, float HdotV, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - HdotV, 5.0);
}

vec3 CalculateRadiance(vec3 lightDir)
{
	float dist = length(fLightPos - fPos);
	float normalizedDist = dist / Light.Range;
	float attenuation = 1.0 / (10.0 * normalizedDist * normalizedDist);
	float centerAngle = dot(lightDir, normalize(-fLightOrienation));
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

vec3 CalculateAmbient(float NdotV, float roughness, float metallic, float ambientOclussion, vec3 F0, vec3 normal, vec3 reflectionVector, vec3 albedo)
{
	vec3 kS = FresnelSchlickRoughness(F0, NdotV, roughness);
	vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

	vec3 irradiance = texture(texIrradiance, fTBN * normal).xyz;
	vec3 albedoColor = kD * irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0;
    vec3 environmentalColor = textureLod(texEnvironmental, fTBN * reflectionVector, roughness * MAX_REFLECTION_LOD).xyz;
	vec2 environmentBRDF = texture(texBRDFIntegration, vec2(NdotV, roughness)).xy;
	vec3 specularColor = environmentalColor * (kS * environmentBRDF.x + environmentBRDF.y);

	return (albedoColor + specularColor) * ambientOclussion;
}

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
	vec3 lightDir = normalize(fLightPos - fPos);
	vec3 halfVector = normalize(lightDir + viewDir);

	vec2 texCoords = ParallaxMapping(fTexCoords, viewDir);

	vec4 albedoTexture = texture(texAlbedo, texCoords);
	vec3 normalTexture = texture(texNormal, texCoords).xyz;
	vec4 metallicTexture = texture(texMetallic, texCoords);
	float occlusionTexture = texture(texOcclusion, texCoords).x;
	vec3 emissionColor = texture(texEmission, texCoords).xyz;

	vec3 albedo = albedoTexture.xyz;
	float ambientOclussion = (Material.SpecularWorkflow || Material.UseOcclusionMap) ? occlusionTexture : metallicTexture.x;
	float roughness = Material.SpecularWorkflow ? 1.0 - metallicTexture.w : metallicTexture.y;
	float metallic = Material.SpecularWorkflow ? 0.0 : metallicTexture.z;
	vec3 normal = normalize(normalTexture * 2.0 - 1.0); normal.y = -normal.y;

	vec3 reflectionVector = reflect(-viewDir, normal);
	float NdotV = max(dot(normal, viewDir), 0.0);
	float NdotL = max(dot(normal, lightDir), 0.0);
	float NdotH = max(dot(normal, halfVector), 0.0);
	float HdotV = max(dot(halfVector, viewDir), 0.0);

	vec3 radiance = CalculateRadiance(lightDir);

	vec3 F0 = Material.SpecularWorkflow ? metallicTexture.xyz : mix(vec3(0.04), albedo, metallic);
	vec3 kS = FresnelSchlick(F0, HdotV);
	vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
	
	vec3 diffuseColor = kD * albedo / PI * radiance * NdotL;
	
	vec3 specularColor = CookTorranceLobe(NdotV, NdotL, NdotH, roughness, kS) * radiance * NdotL;

	vec3 ambientColor = CalculateAmbient(NdotV, roughness, metallic, ambientOclussion, F0, normal, reflectionVector, albedo);

	float lightToGeometryAngle = dot(vec3(0.0, 0.0, 1.0), lightDir);
	if (lightToGeometryAngle < 0.0)
	{	// we are on the opposite side from light, slowly fade away so we dont have illuminated hidden surface due to normal mapping
		float oppositeSideCoeff = pow(1 + lightToGeometryAngle, 3);
		diffuseColor *= oppositeSideCoeff;
		specularColor *= oppositeSideCoeff;
	}

	vec3 color = diffuseColor + specularColor + emissionColor + ambientColor;

	float alpha = albedoTexture.w;
	if(alpha < 0.1)
        discard;

	gl_FragColor = vec4(color, alpha);
}