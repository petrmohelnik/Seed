#version 460

layout(binding = 1) uniform CameraBlock
{
	mat4 projection;
	vec3 viewPos;
};

layout(binding = 2) uniform ModelBlock
{
	mat3 modelView;
	mat3 tiModelView;
};

layout(binding = 3) uniform MaterialBlock
{
	vec3 placeholder;
};

layout(binding = 4) uniform LightBlock
{
	vec3 pos;
	vec3 color;
	vec3 ambient;
} lights[10];

in vec3 vPos;
in vec3 vNorm;
in vec2 vTexCoord;

out vec3 fPos;
out vec3 fNorm;
out vec2 fTexCoord;

void main()
{
	fPos = modelView * vPos;
	fNorm = vNorm;
	fTexCoord = vTexCoord;

	gl_Position = mat4(modelView) * projection * vec4(vPos, 1.0);
}