#include "vertexAttributeLayout.h"

GLuint VertexAttributeLayout::getSizeOfType(GLenum type)
{
	switch (type)
	{
	case GL_FLOAT:
		return sizeof(GLfloat);
	default:
		return 0;
	}
}

const std::vector<VertexAttribute>& const VertexAttributeLayout::getAttributes()
{
	return attributes;
}

GLsizei VertexAttributeLayout::getStride()
{
	return totalStride;
}
