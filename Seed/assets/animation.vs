#version 140

uniform mat4 mvp;
uniform mat3 m;
uniform mat4 T[25];
uniform mat3 ti_T[25];

in vec3 v_pos;
in vec3 v_norm;
in vec2 v_texCoord;
in vec4 v_weights;
in ivec4 v_joints;

out vec3 f_pos;
out vec3 f_norm;
out vec2 f_texCoord;

void main()
{
	f_texCoord = v_texCoord;

	vec4 pos = vec4(0.0);
	pos += T[v_joints.x] * vec4(v_pos, 1.0) * v_weights.x;
	pos += T[v_joints.y] * vec4(v_pos, 1.0) * v_weights.y;
	pos += T[v_joints.z] * vec4(v_pos, 1.0) * v_weights.z;
	pos += T[v_joints.w] * vec4(v_pos, 1.0) * v_weights.w;
	vec3 norm = vec3(0.0);
	norm += ti_T[v_joints.x] * v_norm * v_weights.x;
	norm += ti_T[v_joints.y] * v_norm * v_weights.y;
	norm += ti_T[v_joints.z] * v_norm * v_weights.z;
	norm += ti_T[v_joints.w] * v_norm * v_weights.w;
	
	f_norm = normalize(norm);
	f_pos = m * pos.xyz;
	gl_Position = mvp * vec4(pos.xyz, 1.0);
	//gl_Position = mvp * globalM[v_joints.x] * inverseM[v_joints.x] * vec4(v_pos, 1.0);
}