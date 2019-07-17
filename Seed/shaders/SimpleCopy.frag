#version 460

layout(binding = 11) uniform sampler2D texScene;

in vec2 fTexCoords;

layout(location = 0) out vec4 gl_FragColor;

void main()
{ 
    vec3 color = texture(texScene, fTexCoords).xyz;

    gl_FragColor = vec4(color, 1.0);
}