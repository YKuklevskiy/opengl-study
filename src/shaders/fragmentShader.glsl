#version 330 core

out vec4 FragColor;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

uniform sampler2D _texture;
uniform sampler2D _normalTexture;
uniform float time;
uniform float ambientLight;
uniform vec3 lightPosition;

const float modifier = 2;

void main()
{
    vec4 tex = texture(_texture, texCoord * modifier);
    vec4 norm = texture(_normalTexture, texCoord * modifier);

    vec3 vecFromLightToPos = normalize(position - lightPosition);

    float diffuseLight = max(0.0f, dot(-vecFromLightToPos, normal));

    float resultLighting = ambientLight + diffuseLight;

    FragColor = resultLighting * mix(tex, norm, time);
} 