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

void VBO::bufferData(std::vector<Vertex>& vertexArray)
{
	bind(); // TODO maybe implement observer pattern and only buffer if this VBO is currently bound
	glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(Vertex), vertexArray.data(), GL_STATIC_DRAW);
	_bufferSize = vertexArray.size() * sizeof(Vertex) / sizeof(GLfloat);
}

size_t VBO::getBufferSize() const
{
	return _bufferSize;
}
