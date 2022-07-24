#pragma once

#include <glad/glad.h>

#include "vertexAttributeLayout.h"

class VAO
{
public:
	VAO();

	void bind() const;
	void unbind() const;

	void setupVertexAttributes(VertexAttributeLayout layout);

private:
	GLuint _id;
};