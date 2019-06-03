#version 460

layout(std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPos;
};

layout(std140, binding = 1) uniform LightBlock
{
	vec3 lightPos;
	vec3 lightColor;
	vec3 lightAttenuationCoeffs;
	vec2 lightCutoff;
	vec3 lightOrientation;
	vec3 lightAmbient;
};

layout(std140, binding = 2) uniform ModelBlock
{
	mat4 model;
	mat3 tiModel;
};

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec3 vTang;
layout(location = 3) in vec3 vBitang;
layout(location = 4) in vec2 vTexCoords;

out vec3 fPos;
out vec3 fViewPos;
out vec3 fLightPos;
out vec3 fLightOrienation;
out vec2 fTexCoords;
out mat3 fTBN;

void main()
{
	vec3 T = normalize(tiModel * vTang);
	vec3 B = normalize(tiModel * vBitang);
	vec3 N = normalize(tiModel * vNorm);
	mat3 TBN = transpose(mat3(T, B, N)); //transpose equals inverse for orthogonal matrices
	fTBN = TBN;

	fPos = TBN * vec3(model * vec4(vPos, 1.0));
	fViewPos = TBN * viewPos;
	fLightPos = TBN * lightPos;
	fLightOrienation = TBN * lightOrientation;
	fTexCoords = vTexCoords;

	gl_Position = projection * view * model * vec4(vPos, 1.0);
}