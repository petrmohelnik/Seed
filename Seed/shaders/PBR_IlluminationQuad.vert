#version 460

layout(std140, binding = 0) uniform CameraBlock
{
    mat4 view;
    mat4 projection;
    vec3 viewPos;
};

layout(location = 0) in vec3 vPos;

out vec3 fViewPos;
out mat4 fViewProjectionInverseMatrix;

void main()
{
    fViewPos = viewPos;
    fViewProjectionInverseMatrix = inverse(projection * view);

    gl_Position = vec4(vPos, 1.0);
}