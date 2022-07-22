#version 460

layout(binding = 11) uniform samplerCube texEnvironmental;

in vec3 fTexCoords;

layout(location = 0) out vec4 FragColor;

void main()
{
    FragColor = texture(texEnvironmental, fTexCoords);
}