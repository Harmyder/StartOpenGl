#version 450 core

out  VS_OUT
{
    vec4 vs_color;
} fs_in;

out vec4 color;

void main(void)
{
    color = fs_in.vs_color;
}