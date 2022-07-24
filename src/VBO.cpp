#include "VBO.h"

VBO::VBO()
{
	glGenBuffers(1, &_id);
}

void VBO::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VBO::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
