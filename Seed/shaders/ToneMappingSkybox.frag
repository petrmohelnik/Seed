#version 460

layout(binding = 11) uniform samplerCube texScene;

in vec3 fPos;

layout(location = 0) out vec4 FragColor;

vec3 ToneMapping(vec3 color)
{
    return color / (color + vec3(1.0));
}

vec3 GammaCorrection(vec3 color)
{
    return pow(color, vec3(1.0/2.2));
}

const mat3 ACESInputMat =
{
    {0.59719, 0.35458, 0.04823},
    {0.07600, 0.90834, 0.01566},
    {0.02840, 0.13383, 0.83777}
};

const mat3 ACESOutputMat =
{
    { 1.60475, -0.53108, -0.07367},
    {-0.10208,  1.10813, -0.00605},
    {-0.00327, -0.07276,  1.07602}
};

vec3 RRTAndODTFit(vec3 v)
{
    vec3 a = v * (v + 0.0245786f) - 0.000090537f;
    vec3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

vec3 ACESFitted(vec3 color)
{
    return RRTAndODTFit(color * ACESInputMat) * ACESOutputMat;
}

void main()
{ 
    vec3 color = texture(texScene, fPos).xyz;
    color = ACESFitted(color);
    color = GammaCorrection(color);

    FragColor = vec4(color, 1.0);
}