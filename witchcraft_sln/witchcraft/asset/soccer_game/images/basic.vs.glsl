#version 330 core
layout(location=0) in vec3 position;
layout(location=1) in vec4 vs_color;
out vec4 fs_color;
void main()
{
    gl_Position.xyz = position;
    gl_Position.w = 1.0f;
    fs_color = vs_color;
}
