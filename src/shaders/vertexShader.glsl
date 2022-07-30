#version 330 core
layout (location=0) in vec3 pos;
layout (location=1) in vec2 tex_coord;
layout (location=2) in vec3 normals;

out vec3 position;
out vec2 texCoord;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0f);

	texCoord = tex_coord;
	position = vec3(view * model * vec4(pos, 1.0f));
	normal = normalMatrix * normals;
}