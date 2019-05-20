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
in vec3 fViewPos;
in vec3 fLightPos;
in vec2 fTexCoord;

layout(location = 0) out vec4 gl_FragColor;

vec3 fresnel(vec3 F0, float HdotL)
{
  return F0 + (1.0 - F0) * pow(1.0 - HdotL, 5.0); //Schlick's approximation
}

vec3 BlinnPhongLobe(vec3 viewDir, vec3 lightDir, vec3 normal, vec3 albedo, float shininess)
{
  vec3 halfVector = normalize(lightDir + viewDir);
  float blinnTerm = clamp(dot(normal, halfVector), 0.0, 1.0);
  return clamp(fresnel(albedo, dot(halfVector, viewDir)) * pow(blinnTerm, shininess), 0.0, 1.0);
}

void main()
{
	vec3 viewDir = normalize(fViewPos - fPos);
	vec3 lightDir = normalize(fLightPos - fPos);

	vec3 normalTexture = texture(texNormal, fTexCoord).xyz;
	vec3 normal = normalize(normalTexture * 2.0 - 1.0);
	normal.y = -normal.y;

	vec4 diffuseTexture = texture(texDiffuse, fTexCoord);
	vec3 diffuseReflection = lightColor * max(0.0, dot(normal, lightDir));
	vec3 diffuseColor = (lightAmbient + diffuseReflection) * diffuseTexture.xyz;
	
	vec4 specularTexture = texture(texSpecular, fTexCoord);
	vec3 specularReflection = vec3(0.0);
	if (dot(normal, lightDir) > 0.0)
	{
		specularReflection = lightColor * BlinnPhongLobe(viewDir, lightDir, normal, vec3(0.5), pow(2.0, 13.0 * specularTexture.w));
	}
	vec3 specularColor = specularReflection * specularTexture.xyz;

	float alpha = diffuseTexture.w + (specularColor.r + specularColor.g + specularColor.b) * 0.3333333334;
	if(alpha < 0.05)
        discard;
	gl_FragColor = vec4(diffuseColor + specularColor, alpha);
}