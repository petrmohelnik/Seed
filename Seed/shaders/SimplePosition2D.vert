#version 460

layout(location = 0) in vec3 vPos;

out vec2 fTexCoords;

void main()
{
    fTexCoords = vec2(max(vPos.x, 0.0), max(vPos.y, 0.0));
	
    gl_Position = vec4(vPos, 1.0);
}