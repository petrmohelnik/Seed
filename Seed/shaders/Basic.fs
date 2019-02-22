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

layout(binding = 0) uniform sampler2D texDiffuse;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texSpecular;

in vec3 fPos;
in vec3 fNorm;
in vec2 fTexCoord;

out vec4 gl_FragColor;

void main()
{
	float shininess = texture(texSpecular, fTexCoord).x * 100.0;

	vec3 viewDir = normalize(viewPos.xyz - fPos).xyz;
	vec3 lightDir = normalize(lights[0].pos.xyz - fPos);
	vec3 normal = normalize(mat3(tiModel) * fNorm);

	vec3 diffuseReflection = lights[0].color.xyz * max(0.0, dot(normal, lightDir));

	vec3 specularReflection = vec3(0.0, 0.0, 0.0);
	if (dot(normal, lightDir) > 0.0)
	{
		specularReflection = lights[0].color.xyz * pow(max(0.0, dot(reflect(-lightDir, normal), viewDir)), shininess);
	}

	gl_FragColor = vec4(vec3(diffuseReflection + specularReflection + lights[0].ambient.xyz) * vec3(texture(texDiffuse, fTexCoord)), 1.0);
}