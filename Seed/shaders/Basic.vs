#version 460

layout(std140, binding = 0) uniform CameraBlock
{
	mat4 projection;
	mat4 view;
	vec4 viewPos;
};

layout(std140, binding = 1) uniform ModelBlock
{
	mat4 model;
	mat4 tiModel;
};

layout(std140, binding = 2) uniform MaterialBlock
{
	vec4 placeholder;
};

layout(std140, binding = 3) uniform LightBlock
{
	vec4 pos;
	vec4 color;
	vec4 ambient;
} lights[10];

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTexCoord;

out vec3 fPos;
out vec3 fNorm;
out vec2 fTexCoord;

void main()
{
	fPos = mat3(model) * vPos;
	fNorm = vNorm;
	fTexCoord = vTexCoord;

	gl_Position = projection * view * model * vec4(vPos, 1.0);
}