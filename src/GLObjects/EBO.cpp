#include <GLObjects/EBO.h>

EBO::EBO()
{
	glGenBuffers(1, &_id);
}

EBO::~EBO()
{
	unbind();
	glDeleteBuffers(1, &_id);
}

void EBO::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}

void EBO::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

size_t EBO::getBufferSize() const
{
	return _bufferSize;
}
