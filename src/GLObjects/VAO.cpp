#include <GLObjects/VAO.h>

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

void VAO::setupVertexAttributes(VertexAttributeLayout layout)
{
	bind(); // TODO maybe implement observer pattern and only setup if this VAO is currently bound
	_layoutValueCount = 0;

	auto attributes = layout.getAttributes();
	GLsizei stride = layout.getStride();
	GLuint offset = 0;

	for (int i = 0; i < attributes.size(); i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attributes[i].size, attributes[i].type, attributes[i].normalized, stride, (void*) offset);
		
		GLuint sizeOfAttributeType = VertexAttributeLayout::getSizeOfType(attributes[i].type);

		offset += attributes[i].size * sizeOfAttributeType;
		_layoutValueCount += attributes[i].size;
	}
}

GLuint VAO::getLayoutSize() const
{
	return _layoutValueCount;
}
