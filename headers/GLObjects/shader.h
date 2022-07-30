#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

using std::string;

class Shader
{
public:
	// Shader paths are looked up relatively to the shader folder.
	// For example, if your shader is right in src/shaders/ folder,
	// you just need to pass the name of the file.
	Shader(string vertexShaderPath, string fragmentShaderPath);
	~Shader();

	void use() const;
	
	void setBool(const string& const name, GLboolean value) const;
	void setInt(const string& const name, GLint value) const;
	void setFloat(const string& const name, GLfloat value) const;
	void setMat3f(const string& const name, const glm::mat3& value) const;
	void setMat4f(const string& const name, const glm::mat4& value) const;
	void setVec3f(const string& const name, const glm::vec3& value) const;

	GLuint getID() const;
	bool isValid() const;

private:
	GLuint _id;
	bool _valid = true;
	static const string SHADER_FOLDER_PATH;

	string _readFile(string fileName);
	GLuint _initShader(GLenum type, const char* shaderSource);
	void _buildShaderProgram(GLuint vertexShader, GLuint fragmentShader);
};