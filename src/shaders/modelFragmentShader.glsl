#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 normal;
in vec3 position;

struct Material
{
    sampler2D texture_diffuse[8];
    sampler2D texture_specular[8];
};

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

const float ambient = 0.15f;
const float shininess = 32;

// 32 units
const float attenuation_c = 1.0f;
const float attenuation_l = 0.14f;
const float attenuation_q = 0.07f;

float pointLightAttenuation(float distance)
{
    return 1.0f / (attenuation_c + attenuation_l * distance + attenuation_q * distance * distance);
}

void main()
{    
    vec4 tex = texture(material.texture_diffuse[0], TexCoords);
    vec4 specularTex = texture(material.texture_specular[0], TexCoords);

    vec3 n_normal = normalize(normal);
    vec3 lightToPosInverted = normalize(position - light.position);
    vec3 reflectLightVec = reflect(lightToPosInverted, n_normal);
    vec3 viewToPos = normalize(-position);

    vec3 ambientLight = light.ambient * vec3(ambient);
    vec3 diffuseLight = light.diffuse * vec3(tex) * max(0.0f, dot(-lightToPosInverted, n_normal));
    vec3 specularLight = light.specular * vec3(specularTex) * pow(max(0.0, dot(viewToPos, reflectLightVec)), shininess);

    vec3 resultLighting = ambientLight + diffuseLight + specularLight;
    resultLighting *= pointLightAttenuation(length(position-light.position));

    FragColor = vec4(resultLighting, 1.0f);
}