#version 330 core

out vec4 FragColor;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

uniform sampler2D _texture;
uniform sampler2D _normalTexture;
uniform float time;
uniform float ambientLight;
uniform float specularStrength;
uniform vec3 lightPosition;

const float modifier = 2;

void main()
{
    vec4 tex = texture(_texture, texCoord * modifier);
    vec4 norm = texture(_normalTexture, texCoord * modifier);

    vec3 n_normal = normalize(normal);
    vec3 lightToPosInverted = normalize(position - lightPosition);
    vec3 reflectLightVec = reflect(lightToPosInverted, n_normal);
    vec3 viewToPos = normalize(-position);
    float shininess = 32;

    float diffuseLight = max(0.0f, dot(-lightToPosInverted, n_normal));
    float specularLight = specularStrength * pow(max(0.0, dot(viewToPos, reflectLightVec)), shininess);

    float resultLighting = ambientLight + diffuseLight + specularLight;

    vec4 finalTexture = mix(tex, norm, time);
    FragColor = resultLighting * finalTexture;
} 