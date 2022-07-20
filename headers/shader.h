#pragma once

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using std::cout;
using std::string;

class Shader
{
public:
	// Shader paths are looked up relatively to the shader folder.
	// For example, if your shader is in the root of shaders folder,
	// you just need to pass the name of the file.
	Shader(string vertexShaderPath, string fragmentShaderPath);

	void use();
	
	void setBool(const string& const name, GLboolean value);
	void setInt(const string& const name, GLint value);
	void setFloat(const string& const name, GLfloat value);

	GLuint getID();
	bool isValid();

private:
	GLuint _id;
	bool _valid;
	static const string SHADER_FOLDER_PATH;

	string _readFile(string fileName);
	GLuint _initShader(GLenum type, const char* shaderSource);
	void _buildShaderProgram(GLuint vertexShader, GLuint fragmentShader);
};