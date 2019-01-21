//TODO: osvetleni, barva/textura
#version 140

uniform mat3 ti_m;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 ambientLight;
uniform sampler2D texDifSampler;

in vec3 f_pos;
in vec3 f_norm;
in vec2 f_texCoord;

out vec4 gl_FragColor;

void main()
{
	//vec3 color = vec3(0.7, 0.7, 0.7);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	float shininess = 100.0;

	vec3 viewDir = normalize(viewPos - f_pos);
	vec3 lightDir= normalize(lightPos - f_pos);
	vec3 normal = normalize(ti_m * f_norm);

	vec3 ambientReflection = ambientLight;

	vec3 diffuseReflection = lightColor * max(0.0, dot(normal, lightDir));

	vec3 specularReflection;
	if (dot(normal, lightDir) < 0.0) // light source on the wrong side?
	{
		specularReflection = vec3(0.0, 0.0, 0.0); // no specular reflection
	}
	else
	{
		specularReflection = lightColor * pow(max(0.0, dot(reflect(-lightDir, normal), viewDir)), shininess);
	}

	gl_FragColor = vec4(vec3(diffuseReflection + specularReflection + ambientReflection) * vec3(texture(texDifSampler, f_texCoord)), 1.0);
}