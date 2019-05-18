#version 460

layout(std140, binding = 0) uniform CameraBlock
{
	mat4 projection;
	mat4 view;
	vec3 viewPos;
};

layout(std140, binding = 1) uniform LightBlock
{
	vec3 lightPos;
	vec3 lightColor;
	vec3 lightAmbient;
};

layout(std140, binding = 2) uniform ModelBlock
{
	mat4 model;
	mat3 tiModel;
};

layout(std140, binding = 3) uniform MaterialBlock
{
	vec4 placeholder;
};

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec3 vTang;
layout(location = 3) in vec3 vBitang;
layout(location = 4) in vec2 vTexCoord;

out vec3 fPos;
out vec3 fViewPos;
out vec3 fLightPos;
out vec2 fTexCoord;

void main()
{
	vec3 T = normalize(tiModel * vTang);
	vec3 N = normalize(tiModel * vNorm);
	//T = normalize(T - dot(T, N) * N); //re-orthogonalize T with respect to N
	//vec3 B = cross(N, T);
	vec3 B = normalize(tiModel * vBitang);
	//opengl has reversed y in textures as opposed to how normal maps are ceated
	mat3 TBN = transpose(mat3(T, -B, N)); //transpose equals inverse for orthogonal matrices

	fPos = TBN * mat3(model) * vPos;
	fViewPos = TBN * viewPos;
	fLightPos = TBN * lightPos;
	fTexCoord = vTexCoord;

	gl_Position = projection * view * model * vec4(vPos, 1.0);
}