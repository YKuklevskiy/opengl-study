#version 330 core
layout (location=0) in vec2 pos;
layout (location=1) in vec3 color;

out vec3 col;

uniform float offset;

void main()
{
	gl_Position = vec4(pos.x + offset, pos.y, 0.0f, 1.0f);
	col = color;
}