#version 460

uniform mat4 mvp;
uniform mat3 m;

in vec3 v_pos;
in vec3 v_norm;
in vec2 v_texCoord;

out vec3 f_pos;
out vec3 f_norm;
out vec2 f_texCoord;

void main()
{
	f_pos = m * v_pos;
	f_norm = v_norm;
	f_texCoord = v_texCoord;

	gl_Position = mvp * vec4(v_pos, 1.0);
}