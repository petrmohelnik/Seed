#version 460 core

in vec4 FragPos;

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

layout(location = 0) out float gl_FragDepth;

void main()
{
    float lightDistance = length(FragPos.xyz - Light.Pos);
    lightDistance /= Light.Range;
    
    gl_FragDepth = lightDistance;
} 