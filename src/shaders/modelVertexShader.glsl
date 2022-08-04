#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 normal;
out vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
    TexCoords = aTexCoords;

    vec4 viewPosition = view * model * vec4(aPos, 1.0);

    gl_Position = projection * viewPosition;
    position = vec3(viewPosition);

    normal = normalMatrix * aNormal;
}