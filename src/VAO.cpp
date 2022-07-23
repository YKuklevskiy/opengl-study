#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &_id);
}

void VAO::bind()
{
	glBindVertexArray(_id);
}

void VAO::setupVertexAttribute(VertexAttribute attribute)
{
	bind(); // TODO maybe implement observer pattern and only setup if this VAO is currently bound
	glEnableVertexAttribArray(attribute.index);
	glVertexAttribPointer(
		attribute.index,
		attribute.size,
		attribute.type,
		attribute.normalized,
		attribute.stride,
		attribute.offset
	);
}
