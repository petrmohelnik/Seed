#version 460 core

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
    float SpotAngle;
    float SpotAngleScale;
    float SpotAngleOffset;
    float ShadowFarPlane;
    float ViewPortScale;
    uint Type;
} Light;

in vec4 WorldPos;

void main()
{
    gl_FragDepth = length(WorldPos.xyz - Light.Pos) / Light.ShadowFarPlane;
} 