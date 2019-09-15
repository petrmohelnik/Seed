#version 460

const uint DirectionalLight    = 0x00000001u;
const uint PointLight          = 0x00000002u;
const uint SpotLight           = 0x00000004u;
const uint IsShadowCasterLight = 0x00000008u;

layout(std140, binding = 1) uniform LightBlock
{
    mat4 ViewMatrix;
    mat4 ProjectionMatrix;
	vec3 Pos;
	vec3 Color;
	vec3 Orientation;
	float Range;
	float Intensity;
    float SizeUV;
	float SpotAngleScale;
	float SpotAngleOffset;
    uint Type;
} Light;

layout(binding = 0) uniform sampler2D texColor;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texDepth;
layout(binding = 3) uniform sampler2D texMetallic;
layout(binding = 7) uniform sampler2DShadow texShadowLerp;

layout(location = 0) uniform ivec2 screenSize;

in vec3 fViewPos;
in mat4 fViewProjectionInverseMatrix;

layout(location = 0) out vec4 gl_FragColor;

const float PI = 3.14159265359;

vec3 FresnelSchlick(vec3 F0, float HdotV)
{
	return F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);
}

vec3 CalculateRadiance(vec3 lightDir, vec3 worldPos)
{
    if(bool(Light.Type & DirectionalLight))
        return Light.Color * Light.Intensity;

	float dist = length(Light.Pos - worldPos);
	float normalizedDist = Light.Range == 0.0 ? 0.0 : dist / Light.Range;
	float attenuation = pow(clamp(1.0 - pow(normalizedDist, 4), 0.0, 1.0), 2) / max(dist * dist, 0.01 * 0.01);

    if(bool(Light.Type & SpotLight))
    {
	    float centerAngle = dot(lightDir, normalize(-Light.Orientation));
	    float angleAttenuation = clamp(Light.SpotAngleOffset + centerAngle * Light.SpotAngleScale, 0.0, 1.0);
        angleAttenuation *= angleAttenuation;
        attenuation *= angleAttenuation;
    }

    if(attenuation <= 0.0)
        discard;

	return Light.Color * Light.Intensity * attenuation;
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

vec2 VogelDiskCoords(int index, int count, float phi)
{
    float r = sqrt(index + 0.5) / sqrt(count);
    float theta = index * 2.4 + phi;
    return vec2(r * cos(theta), r * sin(theta));
}

float InterleavedGradientNoise(vec2 texCoords)
{
    vec3 seed = vec3(0.06711056f, 0.00583715f, 52.9829189f);
    return fract(seed.z * fract(dot(texCoords, seed.xy)));
}

float CalculateShadow(vec3 worldPos, vec3 normal, vec3 lightDir, vec2 texCoords)
{
    vec4 lightSpacePosClipSpace = Light.ProjectionMatrix * Light.ViewMatrix * vec4(worldPos, 1.0);
    vec3 lightSpacePos = lightSpacePosClipSpace.xyz / lightSpacePosClipSpace.w;
    lightSpacePos = lightSpacePos * 0.5 + 0.5;

    if(lightSpacePos.z > 1.0)
        return 0.0;

    float bias = mix(0.001, 0.00001, dot(normal, lightDir));
    float gradientNoise = PI * 2 * InterleavedGradientNoise(gl_FragCoord.xy);
    vec2 filterRadius = 1.0 / textureSize(texShadowLerp, 0) * 20.0 * Light.SizeUV;

    float shadow = 0.0;
    for(int i = 0; i < 24; i++)
    {
        vec2 sampleCoords = VogelDiskCoords(i, 24, gradientNoise) * filterRadius;
        shadow += 1.0 - texture(texShadowLerp, vec3(lightSpacePos.xy + sampleCoords, lightSpacePos.z + bias)).r;
    }
    shadow /= 24.0f;

    return shadow;
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

    vec3 worldPos = CalculateWorldPosition(texCoords);
	vec3 lightDir = bool(Light.Type & DirectionalLight) ? normalize(Light.Orientation) : normalize(Light.Pos - worldPos);
	vec3 radiance = CalculateRadiance(lightDir, worldPos);

	vec4 colorTexture = texture(texColor, texCoords);
	vec4 normalTexture = texture(texNormal, texCoords);
	vec4 metallicTexture = texture(texMetallic, texCoords);

	vec3 albedo = colorTexture.xyz;
    bool isSpecularWorkflow = normalTexture.w != -1.0;
	float roughness = isSpecularWorkflow ? metallicTexture.w : metallicTexture.y;
	float metallic = isSpecularWorkflow ? 0.0 : metallicTexture.z;
	vec3 normal = normalize(normalTexture.xyz);

	vec3 viewDir = normalize(fViewPos - worldPos);
	vec3 halfVector = normalize(lightDir + viewDir);
	float NdotV = max(dot(normal, viewDir), 0.0);
	float NdotL = max(dot(normal, lightDir), 0.0);
	float NdotH = max(dot(normal, halfVector), 0.0);
	float HdotV = max(dot(halfVector, viewDir), 0.0);

	vec3 F0 = isSpecularWorkflow ? metallicTexture.xyz : mix(vec3(0.04), albedo, metallic);
	vec3 kS = FresnelSchlick(F0, HdotV);
	vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);
	
	vec3 diffuseColor = kD * albedo / PI * radiance * NdotL;
	vec3 specularColor = CookTorranceLobe(NdotV, NdotL, NdotH, roughness, kS) * radiance * NdotL;

    float shadow = 0.0f;
    if(bool(Light.Type & IsShadowCasterLight))
        shadow = CalculateShadow(worldPos, normal, lightDir, texCoords);

    gl_FragColor = vec4((1.0 - shadow) * (diffuseColor + specularColor), 1.0);
}