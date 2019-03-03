#version 460

layout(std140, binding = 0) uniform CameraBlock
{
	mat4 projection;
	mat4 view;
	vec4 viewPos;
};

layout(std140, binding = 1) uniform LightBlock
{
	vec4 lightPos;
	vec4 lightColor;
	vec4 lightAmbient;
};

layout(std140, binding = 2) uniform ModelBlock
{
	mat4 model;
	mat4 tiModel;
};

layout(std140, binding = 3) uniform MaterialBlock
{
	vec4 placeholder;
};

layout(binding = 0) uniform sampler2D texDiffuse;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texSpecular;

in vec3 fPos;
in vec3 fNorm;
in vec2 fTexCoord;

layout(location = 0) out vec4 gl_FragColor;

void main()
{
	vec3 viewDir = normalize(viewPos.xyz - fPos).xyz;
	vec3 lightDir = normalize(lightPos.xyz - fPos);
	vec3 normal = normalize(mat3(tiModel) * fNorm);

	vec3 diffuseReflection = lightColor.xyz * max(0.0, dot(normal, lightDir));

	vec3 specularReflection = vec3(0.0, 0.0, 0.0);
	if (dot(normal, lightDir) > 0.0)
	{
		specularReflection = lightColor.xyz * texture(texSpecular, fTexCoord).xyz * pow(max(0.0, dot(reflect(-lightDir, normal), viewDir)), 100.0);
	}

	gl_FragColor = vec4(vec3(diffuseReflection + specularReflection + lightAmbient.xyz) * vec3(texture(texDiffuse, fTexCoord)), 1.0);
}