#pragma once

#include <glad/glad.h>

class EBO
{
public:
	EBO();
	~EBO();

	void bind() const;
	void unbind() const;

	template <size_t n>
	void bufferData(GLuint (&indexArray)[n]);

	size_t getBufferSize() const;

private:
	GLuint _id;
	size_t _bufferSize = 0;
};

template<size_t n>
inline void EBO::bufferData(GLuint (&indexArray)[n])
{
	bind(); // TODO maybe implement observer pattern and only buffer if this VBO is currently bound
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);
	_bufferSize = n;
}