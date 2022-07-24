#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D _texture;

void main()
{
    FragColor = texture(_texture, texCoord * 2);
} 