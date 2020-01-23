#version 460

layout(binding = 11) uniform samplerCube texEnvironmental;

in vec3 fTexCoords;

layout(location = 0) out vec4 gl_FragColor;

void main()
{
    gl_FragColor = texture(texEnvironmental, fTexCoords);
}