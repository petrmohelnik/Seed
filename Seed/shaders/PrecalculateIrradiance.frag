#version 460

layout(binding = 5) uniform samplerCube texEnvironmental;

in vec3 fPos;

out vec4 gl_FragColor;

const float PI = 3.14159265359;

void main()
{		
    vec3 normal = normalize(fPos); // the sample direction equals the hemisphere's orientation 
  
    vec3 irradiance = vec3(0.0);

	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = normalize(cross(up, normal));
	up = normalize(cross(normal, right));

	float sampleDelta = 0.005;
	float numberOfSamples = 0.0; 
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
			// spherical to cartesian (in tangent space)
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			// tangent space to world
			vec3 texCoords = tangentSample.x * right + tangentSample.y * up + tangentSample.z * normal; 

			irradiance += texture(texEnvironmental, texCoords).xyz * cos(theta) * sin(theta);
			numberOfSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / float(numberOfSamples));
  
    gl_FragColor = vec4(irradiance, 1.0);
}