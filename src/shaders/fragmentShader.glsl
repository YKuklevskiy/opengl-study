#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D _texture;
uniform sampler2D _normalTexture;
uniform float time;

const float modifier = 2;

void main()
{
    vec4 tex = texture(_texture, texCoord * modifier);
    vec4 norm = texture(_normalTexture, texCoord * modifier);
    FragColor = mix(tex, norm, time);
} 