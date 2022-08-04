#pragma once

#include <glad/glad.h>

#include <iostream>

using std::cout;
using std::string;

enum TextureType
{
	TEXTURE,
	NORMAL_MAP,
	DIFFUSE_MAP,
	SPECULAR_MAP
};

class Texture
{
public:
	Texture(string fullTexturePath, TextureType type);
	~Texture();

	void bind() const;
	void setFiltering(GLenum minifyingFilter,
		GLenum magnifyingFilter) const;
	void setUnit(GLenum unit);

	bool isValid() const;

	TextureType getType() const;

private:
	GLuint _id;
	GLenum _unit = GL_TEXTURE0;
	TextureType _type;
	bool _valid = true;

	static const string TEXTURE_FOLDER_PATH;

	int _width;
	int _height;
	int _channels;

	GLubyte* readImageFile(string fullTexturePath);
};