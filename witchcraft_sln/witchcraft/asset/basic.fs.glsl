#version 330 core
out vec4 fColor;

in vec3 _color;
in vec2 _uv;

uniform sampler2D _texture;

void main()
{
    fColor = texture(_texture, _uv);
}
