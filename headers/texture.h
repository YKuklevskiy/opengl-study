#pragma once

#include <glad/glad.h>

#include <iostream>

using std::cout;
using std::string;

class Texture
{
public:
	Texture(string texturePath, GLenum unit);

	void bind() const;
	void setFiltering(GLenum minifyingFilter,
		GLenum magnifyingFilter) const;

	bool isValid() const;

private:
	GLuint _id;
	GLenum _unit;
	bool _valid = true;
	static const string TEXTURE_FOLDER_PATH;
	int _width;
	int _height;
	int _channels;

	GLubyte* readImageFile(string fullTexturePath);
};