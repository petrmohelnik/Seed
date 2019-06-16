#version 460

layout(binding = 6) uniform samplerCube texEnvironmental;
layout(location = 0) uniform float roughness;

in vec3 fPos;

layout(location = 0) out vec4 gl_FragColor;

const float PI = 3.14159265359;

float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i) / float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness*roughness;
	
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
	
    // from tangent-space vector to world-space sample vector
    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = normalize(cross(N, tangent));
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
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

void main()
{		
    vec3 normal = normalize(fPos);
    vec3 viewDir = normal;

	const uint SAMPLE_COUNT = 2048u;
    float totalWeight = 0.0;   
    vec3 prefilteredColor = vec3(0.0);     
    for(uint i = 0u; i < SAMPLE_COUNT; i++)
    {
        vec2 sequenceValue = Hammersley(i, SAMPLE_COUNT);
        vec3 halfVector = ImportanceSampleGGX(sequenceValue, normal, roughness);
        vec3 lightDir = normalize(2.0 * dot(viewDir, halfVector) * halfVector - viewDir);

        float NdotL = max(dot(normal, lightDir), 0.0);
        if(NdotL > 0.0)
        {
		    float NdotH = max(dot(normal, halfVector), 0.0);
		    float HdotV = max(dot(halfVector, viewDir), 0.0);
			float distribution = DistributionGGX(NdotH, roughness);
			float pdf = (distribution * NdotH / (4.0 * HdotV)) + 0.0001; 

			float resolution = 2048.0; // resolution of source cubemap (per face)
			float saTexel  = 4.0 * PI / (6.0 * resolution * resolution);
			float saSample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

			float mipLevel = roughness == 0.0 ? 0.0 : 0.5 * log2(saSample / saTexel);

            prefilteredColor += textureLod(texEnvironmental, lightDir, mipLevel).xyz * NdotL;
            totalWeight += NdotL;
        }
    }
    prefilteredColor /= totalWeight;

    gl_FragColor = vec4(prefilteredColor, 1.0);
}