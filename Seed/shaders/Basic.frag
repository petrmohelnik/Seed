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
	vec3 diffuseColor = kD * irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0;
    vec3 environmentalColor = textureLod(texEnvironmental, fTBN * reflectionVector, roughness * MAX_REFLECTION_LOD).xyz;
	vec2 environmentBRDF = texture(texBRDFIntegration, vec2(NdotV, roughness)).xy;
	vec3 specularColor = environmentalColor * (kS * environmentBRDF.x + environmentBRDF.y);

	return (diffuseColor + specularColor) * ambientOclussion;
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    const float minLayers = 8.0;
	const float maxLayers = 32.0;
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * 0.01; 
    vec2 deltaTexCoords = P / numLayers;

	// get initial values
vec2  currentTexCoords     = texCoords;
float currentDepthMapValue = abs(texture(texHeight, currentTexCoords).r - 1.0);
  
while(currentLayerDepth < currentDepthMapValue)
{
    // shift texture coordinates along direction of P
    currentTexCoords -= deltaTexCoords;
    // get depthmap value at current texture coordinates
    currentDepthMapValue = abs(texture(texHeight, currentTexCoords).r - 1.0);  
    // get depth of next layer
    currentLayerDepth += layerDepth;  
}


	// get texture coordinates before collision (reverse operations)
vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

// get depth after and before collision for linear interpolation
float afterDepth  = currentDepthMapValue - currentLayerDepth;
float beforeDepth = abs(texture(texHeight, prevTexCoords).r - 1.0) - currentLayerDepth + layerDepth;
 
// interpolation of texture coordinates
float weight = afterDepth / (afterDepth - beforeDepth);
vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

return finalTexCoords;  
}

void main()
{
	vec3 viewDir = normalize(fViewPos - fPos);
	vec3 lightDir = normalize(fLightPos - fPos);
	vec3 halfVector = normalize(lightDir + viewDir);

	vec2 texCoords = fTexCoords;//ParallaxMapping(fTexCoords, viewDir);
//	if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
//    discard;
//
	vec4 albedoTexture = texture(texAlbedo, texCoords);
	vec3 normalTexture = texture(texNormal, texCoords).xyz;
	vec4 specularTexture = texture(texSpecular, texCoords);
	vec3 emissionColor = texture(texEmission, texCoords).xyz;

	vec3 albedo = pow(albedoTexture.xyz, vec3(2.2));
	float ambientOclussion = isMetallic ? specularTexture.x : 1.0;
	float roughness = isMetallic ? specularTexture.y : 1.0 - specularTexture.w;
	float smoothness = 1.0 - roughness;
	float metallic = isMetallic ? specularTexture.z : 0.0;
	vec3 normal = normalize(normalTexture * 2.0 - 1.0); normal.y = -normal.y;

	vec3 reflectionVector = reflect(-viewDir, normal);
	float NdotV = max(dot(normal, viewDir), 0.0);
	float NdotL = max(dot(normal, lightDir), 0.0);
	float NdotH = max(dot(normal, halfVector), 0.0);
	float HdotV = max(dot(halfVector, viewDir), 0.0);

	vec3 radiance = CalculateRadiance(lightDir);

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

	vec3 color = diffuseColor + specularColor + emissionColor + ambientColor;

	float alpha = albedoTexture.w + (specularColor.r + specularColor.g + specularColor.b) * 0.3333333334;
	alpha = clamp(alpha, 0, 1);
	if(alpha < 0.05)
        discard;

	gl_FragColor = vec4(color, alpha);
}