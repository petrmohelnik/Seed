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

layout(binding = 0) uniform sampler2D texDiffuse;
layout(binding = 1) uniform sampler2D texNormal;
layout(binding = 2) uniform sampler2D texSpecular;

in vec3 fPos;
in vec3 fNorm;
in vec3 fTang;
in vec3 fBitang;
in vec2 fTexCoord;

layout(location = 0) out vec4 gl_FragColor;

void main()
{
	vec3 viewDir = normalize(viewPos - fPos);
	vec3 lightDir = normalize(lightPos - fPos);
	vec3 normal = normalize(tiModel * fNorm);

	vec4 diffuseTexture = texture(texDiffuse, fTexCoord);
	vec3 diffuseReflection = lightColor * max(0.0, dot(normal, lightDir));
	vec3 diffuseColor = (lightAmbient + diffuseReflection) * diffuseTexture.xyz;
	
	vec4 specularTexture = texture(texSpecular, fTexCoord);
	vec3 specularReflection = vec3(0.0, 0.0, 0.0);
	if (dot(normal, lightDir) > 0.0)
	{
		specularReflection = lightColor * pow(max(0.0, dot(reflect(-lightDir, normal), viewDir)), pow(2.0, 13.0 * specularTexture.w));
	}
	vec3 specularColor = specularReflection * specularTexture.xyz;

	gl_FragColor = vec4(diffuseColor + specularColor, diffuseTexture.w);
}