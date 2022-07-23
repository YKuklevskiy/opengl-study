#pragma once

#include <glad/glad.h>

class VBO
{
public:
	VBO();

	void bind();

	template <size_t n, typename T>
	void bufferData(T (&vertexArray)[n]);

private:
	GLuint _id;
};

template<size_t n, typename T>
inline void VBO::bufferData(T(&vertexArray)[n])
{
	bind(); // TODO maybe implement observer pattern and only buffer if this VBO is currently bound
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
}