#pragma once

#include "vertexAttribute.h"
#include <vector>
#include <cassert>

class VertexAttributeLayout
{
public:
	
	static GLuint getSizeOfType(GLenum type);

	template<typename T> 
	inline void AddAttribute(int valueCount)
	{
		// we define template specializations for supported attribute types
		assert(false); 
	}

	template<>
	inline void AddAttribute<GLfloat>(int valueCount)
	{
		attributes.push_back( { valueCount, GL_FLOAT, GL_FALSE } );
		totalStride += valueCount * sizeof(GLfloat);
	}

	const std::vector<VertexAttribute>& const getAttributes() const;

	GLsizei getStride() const;

private:
	std::vector<VertexAttribute> attributes;
	GLsizei totalStride = 0;
};