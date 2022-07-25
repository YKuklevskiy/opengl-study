#pragma once

#include <glad/glad.h>

#include "vertexAttributeLayout.h"

class VAO
{
public:
	VAO();
	~VAO();

	void bind() const;
	void unbind() const;

	void setupVertexAttributes(VertexAttributeLayout layout) const;

private:
	GLuint _id;
};