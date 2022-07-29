#include <GLObjects/VBO.h>

VBO::VBO()
{
	glGenBuffers(1, &_id);
}

VBO::~VBO()
{
	unbind();
	glDeleteBuffers(1, &_id);
}

void VBO::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VBO::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

size_t VBO::getBufferSize() const
{
	return _bufferSize;
}
