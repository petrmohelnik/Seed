#version 460 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 viewProjectionMatrices[6];

out vec4 WorldPos;

void main()
{
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i)
        {
            WorldPos = gl_in[i].gl_Position;
            gl_Position = viewProjectionMatrices[face] * WorldPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  