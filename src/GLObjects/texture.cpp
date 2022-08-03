#include <GLObjects/texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

Texture::Texture(string fullTexturePath, TextureType type)
	: _type(type)
{
	stbi_set_flip_vertically_on_load(true); // temporarily here, should actually be called just once before any of resources loading
	auto* data = readImageFile(fullTexturePath);

	if (_valid)
	{
		glGenTextures(1, &_id);
		bind();
		
		// load data to texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	stbi_image_free(data);
}

Texture::~Texture()
{
	glDeleteTextures(1, &_id);
}

void Texture::bind() const
{
	glActiveTexture(_unit);
	glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::setFiltering(GLenum minifyingFilter, GLenum magnifyingFilter) const
{
	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minifyingFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnifyingFilter);
}

void Texture::setUnit(GLenum unit)
{
	_unit = unit;
}

bool Texture::isValid() const
{
	return _valid;
}

TextureType Texture::getType() const
{
	return _type;
}

GLubyte* Texture::readImageFile(string fullTexturePath)
{
	GLubyte* textureData = stbi_load(fullTexturePath.c_str(), &_width, &_height, &_channels, 4);

	if (textureData) // generate texture
	{
		cout << "SUCCESS: SUCCESSFULLY READ TEXTURE \"" << fullTexturePath << "\"\n";
	}
	else
	{
		cout << "ERROR: COULD NOT READ TEXTURE FILE \"" << fullTexturePath << "\"\n";
		_valid = false;
	}

	return textureData;
}
