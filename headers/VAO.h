#pragma once

#include <glad/glad.h>

#include "vertexAttribute.h"

class VAO
{
public:
	VAO();

	void bind();

	void setupVertexAttribute(VertexAttribute attribute);

private:
	GLuint _id;
};