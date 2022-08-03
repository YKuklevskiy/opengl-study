#pragma once

#include <glad/glad.h>
#include <vector>
#include <GLObjects/vertex.h>
#include <type_traits>

class VBO
{
public:
	VBO();
	~VBO();

	void bind() const;
	void unbind() const;

	// ISSUE: bufferData overloads might result in bloating, 
	// maybe create IDataBuffer interface with getData() and getValueCount() funcs
	template <size_t n, typename T>
	void bufferData(T(&vertexArray)[n]);

	template <class T>
	typename std::enable_if_t<std::is_arithmetic_v<T>, void>
	bufferData(std::vector<T>& vertexArray);

	void bufferData(std::vector<Vertex>& vertexArray);

	size_t getBufferSize() const;

private:
	GLuint _id;
	size_t _bufferSize = 0;
};

template<size_t n, typename T>
inline void VBO::bufferData(T(&vertexArray)[n])
{
	bind(); // TODO maybe implement observer pattern and only buffer if this VBO is currently bound
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
	_bufferSize = n;
}

template <class T>
typename std::enable_if_t<std::is_arithmetic_v<T>, void>
inline VBO::bufferData(std::vector<T>& vertexArray)
{
	bind(); // TODO maybe implement observer pattern and only buffer if this VBO is currently bound
	glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(T), vertexArray.data(), GL_STATIC_DRAW);
	_bufferSize = vertexArray.size();
}