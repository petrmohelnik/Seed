#version 460

layout(std140, binding = 1) uniform LightBlock
{
	vec3 lightPos;
	vec3 lightColor;
	vec3 lightAttenuationCoeffs;
	vec2 lightCutoff;
	vec3 lightOrientation;
	vec3 lightAmbient;
};

layout(std140, binding = 3) uniform MaterialBlock
{
	bool isMetallic;
};

layout(binding = 0) uniform sampler2D texAlbedo;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texHeight;
layout(binding = 3) uniform sampler2D texSpecular;
layout(binding = 4) uniform sampler2D texEmission;
layout(binding = 5) uniform samplerCube texIrradiance;
layout(binding = 6) uniform samplerCube texEnvironmental;
layout(binding = 7) uniform sampler2D texBRDFIntegration;

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

vec3 GammaCorrection(vec3 color)
{
	return pow(color / (color + vec3(1.0)), vec3(1.0/2.2));
}

vec3 BlinnPhongLobe(float NdotH, float shininess, vec3 kS)
{
	return kS * pow(NdotH, pow(2.0, 13.0 * shininess));
}

vec3 CalculateRadiance(vec3 lightDir)
{
	float lightDist = length(fLightPos - fPos);
	float lightAttenuation = 1.0 / (lightAttenuationCoeffs.x + lightAttenuationCoeffs.y * lightDist + lightAttenuationCoeffs.z * lightDist * lightDist);
	float lightAngleSpot = dot(lightDir, normalize(-fLightOrienation));
	float lightSpotIntensity = clamp((lightAngleSpot - lightCutoff.y) / (lightCutoff.x - lightCutoff.y), 0.0, 1.0);
	return lightColor * lightSpotIntensity * lightAttenuation;
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
	vec3 diffuseColor = kD * irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0;
    vec3 environmentalColor = textureLod(texEnvironmental, fTBN * reflectionVector, roughness * MAX_REFLECTION_LOD).xyz;
	vec2 environmentBRDF = texture(texBRDFIntegration, vec2(NdotV, roughness)).xy;
	vec3 specularColor = environmentalColor * (kS * environmentBRDF.x + environmentBRDF.y);

	return (diffuseColor + specularColor) * ambientOclussion;
}

void main()
{
	vec4 albedoTexture = texture(texAlbedo, fTexCoords);
	vec3 normalTexture = texture(texNormal, fTexCoords).xyz;
	vec4 specularTexture = texture(texSpecular, fTexCoords);
	vec3 emissionColor = texture(texEmission, fTexCoords).xyz;

	vec3 viewDir = normalize(fViewPos - fPos);
	vec3 lightDir = normalize(fLightPos - fPos);
	vec3 halfVector = normalize(lightDir + viewDir);
	vec3 normal = normalize(normalTexture * 2.0 - 1.0); normal.y = -normal.y;
	vec3 reflectionVector = reflect(-viewDir, normal);
	float NdotV = max(dot(normal, viewDir), 0.0);
	float NdotL = max(dot(normal, lightDir), 0.0);
	float NdotH = max(dot(normal, halfVector), 0.0);
	float HdotV = max(dot(halfVector, viewDir), 0.0);

	//vec3 environmentalTexture = texture(texEnvironmental, fTBN * reflectionVector).xyz;
	//vec3 environmentalTexture = texture(texEnvironmental, fTBN * normal).xyz;

	vec3 radiance = CalculateRadiance(lightDir);
	vec3 albedo = pow(albedoTexture.xyz, vec3(2.2));
	float roughness = isMetallic ? specularTexture.g : 1.0 - specularTexture.w;
	float smoothness = 1.0 - roughness;
	float metallic = isMetallic ? specularTexture.b : 0.0;
	float ambientOclussion = isMetallic ? specularTexture.r : 1.0;

	vec3 F0 = isMetallic ? mix(vec3(0.04), albedo, metallic) : specularTexture.xyz;
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

	vec3 color = GammaCorrection(diffuseColor + specularColor + emissionColor + ambientColor);

	float alpha = albedoTexture.w + (specularColor.r + specularColor.g + specularColor.b) * 0.3333333334;
	if(alpha < 0.05)
        discard;

	gl_FragColor = vec4(color, alpha);
}