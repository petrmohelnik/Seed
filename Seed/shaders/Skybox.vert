#version 460

layout(std140, binding = 0) uniform CameraBlock
{
	mat4 view;
	mat4 projection;
	vec3 viewPos;
};

layout(location = 0) in vec3 vPos;

out vec3 fTexCoords;

void main()
{
	fTexCoords = vec3(vPos.x, -vPos.y, vPos.z);

	vec4 position = projection * view * vec4(vPos, 1.0);
	gl_Position = position.xyww;
}