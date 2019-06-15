#version 460

layout(binding = 5) uniform sampler2D texEnvironmental;

in vec3 fPos;

out vec4 FragColor;

const vec2 invAtan = vec2(0.1591, 0.3183);

void main()
{		
    vec3 direction = normalize(fPos);
	vec2 uv = vec2(atan(direction.z, direction.x), asin(direction.y));
    uv *= invAtan;
    uv += 0.5;
    
    FragColor = vec4(texture(texEnvironmental, uv).xyz, 1.0);
}