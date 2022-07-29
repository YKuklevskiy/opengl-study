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

	void setupVertexAttributes(VertexAttributeLayout layout);

	GLuint getLayoutSize() const;

private:
	GLuint _id;
	GLuint _layoutValueCount = 0;
};