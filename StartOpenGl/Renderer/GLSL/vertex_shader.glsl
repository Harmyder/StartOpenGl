#version 450 core

layout (location = 0) in vec4 offset;
layout (location = 1) in vec4 color;

uniform mat4 worldViewProj;

out  VS_OUT
{
    vec4 vs_color;
} vs_out;

// input: gl_VertexID
// output: gl_Position
void main(void)
{
    gl_Position = vec4(0.0, 0.0, 0.5, 1.0);

    vs_out.vs_color = color;
}