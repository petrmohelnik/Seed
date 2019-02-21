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

uniform sampler2D texDiffuse;
uniform sampler2D texNormal;
uniform sampler2D texSpecular;

in vec3 fPos;
in vec3 fNorm;
in vec2 fTexCoord;

out vec4 gl_FragColor;

void main()
{
	float shininess = texture(texSpecular, fTexCoord).x * 100.0;

	vec3 viewDir = normalize(viewPos - fPos);
	vec3 lightDir = normalize(lights[0].pos - fPos);
	vec3 normal = normalize(tiModelView * fNorm);

	vec3 diffuseReflection = lights[0].color * max(0.0, dot(normal, lightDir));

	vec3 specularReflection;
	if (dot(normal, lightDir) < 0.0) // light source on the wrong side?
	{
		specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
	}
	else
	{
		specularReflection = lights[0].color * pow(max(0.0, dot(reflect(-lightDir, normal), viewDir)), shininess);
	}

	gl_FragColor = vec4(vec3(diffuseReflection + specularReflection + lights[0].ambient) * vec3(texture(texDiffuse, fTexCoord)), 1.0);
}