#version 330 core

out vec4 FragColor;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

uniform sampler2D _texture;
uniform sampler2D _normalTexture;
uniform float time;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

const float modifier = 2;

void main()
{
    vec4 tex = texture(_texture, texCoord * modifier);
    vec4 norm = texture(_normalTexture, texCoord * modifier);

    vec3 n_normal = normalize(normal);
    vec3 lightToPosInverted = normalize(position - light.position);
    vec3 reflectLightVec = reflect(lightToPosInverted, n_normal);
    vec3 viewToPos = normalize(-position);

    vec3 ambientLight = material.ambient * light.ambient;
    vec3 diffuseLight = material.diffuse * light.diffuse * max(0.0f, dot(-lightToPosInverted, n_normal));
    vec3 specularLight = material.specular * light.specular * pow(max(0.0, dot(viewToPos, reflectLightVec)), material.shininess);

    vec3 resultLighting = ambientLight + diffuseLight + specularLight;

    vec4 finalTexture = mix(tex, norm, time);
    FragColor = vec4(resultLighting, 1.0f) * finalTexture;
} 