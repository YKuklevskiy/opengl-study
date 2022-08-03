#pragma once

#include <glad/glad.h>
#include <vector>
#include <type_traits>

class EBO
{
public:
	EBO();
	~EBO();

	void bind() const;
	void unbind() const;

	// ISSUE: bufferData overloads might result in bloating, 
	// maybe create IDataBuffer interface with getData() and getValueCount() funcs
	template <size_t n>
	void bufferData(GLuint (&indexArray)[n]);

	template <class T>
	typename std::enable_if_t<std::is_arithmetic_v<T>, void>
	bufferData(std::vector<T>& indexArray);

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

template <class T>
typename std::enable_if_t<std::is_arithmetic_v<T>, void>
inline EBO::bufferData(std::vector<T>& indexArray)
{
	bind(); // TODO maybe implement observer pattern and only buffer if this VBO is currently bound
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(T), indexArray.data(), GL_STATIC_DRAW);
	_bufferSize = indexArray.size();
}