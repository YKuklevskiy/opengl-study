#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &_id);
}

VAO::~VAO()
{
	unbind();
	glDeleteVertexArrays(1, &_id);
}

void VAO::bind() const
{
	glBindVertexArray(_id);
}

void VAO::unbind() const
{
	glBindVertexArray(0);
}

void VAO::setupVertexAttributes(VertexAttributeLayout layout) const
{
	bind(); // TODO maybe implement observer pattern and only setup if this VAO is currently bound

	auto attributes = layout.getAttributes();
	GLsizei stride = layout.getStride();
	GLuint offset = 0;

	for (int i = 0; i < attributes.size(); i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attributes[i].size, attributes[i].type, attributes[i].normalized, stride, (void*)offset);
		
		offset += attributes[i].size * VertexAttributeLayout::getSizeOfType(attributes[i].type);
	}
	
}
