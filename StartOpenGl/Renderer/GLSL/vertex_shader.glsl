#version 450 core

layout (location = 0) in vec4 color;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;

uniform mat4 worldViewProj;

out VS_OUT
{
    vec4 vs_color;
} vs_out;

// input: gl_VertexID
// output: gl_Position
void main(void)
{
    gl_Position = worldViewProj * vec4(position, 1.0);

    vs_out.vs_color = color;
}