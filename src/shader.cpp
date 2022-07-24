#include "shader.h"

const string Shader::SHADER_FOLDER_PATH = "src/shaders/";

Shader::Shader(string vertexShaderPath, string fragmentShaderPath)
{
	_valid = true;

	// Load and compile shaders
	string vertexShaderFile = _readFile(SHADER_FOLDER_PATH + vertexShaderPath);
	GLuint vertexShader = _initShader(GL_VERTEX_SHADER, vertexShaderFile.c_str());

	string fragmentShaderFile = _readFile(SHADER_FOLDER_PATH + fragmentShaderPath);
	GLuint fragmentShader = _initShader(GL_FRAGMENT_SHADER, fragmentShaderFile.c_str());

	// Link shaders in shader program
	_buildShaderProgram(vertexShader, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use() const
{
	glUseProgram(_id);
}

void Shader::setBool(const string& const name, GLboolean value) const
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), (GLint)value);
}

void Shader::setInt(const string& const name, GLint value) const
{
	glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void Shader::setFloat(const string& const name, GLfloat value) const
{
	glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

GLuint Shader::getID() const
{
	return _id;
}

bool Shader::isValid() const
{
	return _valid;
}

string Shader::_readFile(string fileName)
{
	std::ifstream fin(fileName);

	if (!fin.is_open())
	{
		cout << "Failed to read file \"" << fileName << "\".\n";
		_valid = false;
		return "";
	}

	std::stringstream buffer;
	buffer << fin.rdbuf();
	fin.close();

	return buffer.str();
}

GLuint Shader::_initShader(GLenum type, const char* shaderSource)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus)
	{
		GLchar shaderCompileInfo[512];
		glGetShaderInfoLog(shader, 512, NULL, shaderCompileInfo);
		cout << "ERROR: SHADER COMPILATION FAILED\n" << shaderCompileInfo << "\n";
		_valid = false;
		return 0;
	}
	else
		cout << "SUCCESS: SHADER COMPILATION SUCCESSFUL\n";

	return shader;
}

void Shader::_buildShaderProgram(GLuint vertexShader, GLuint fragmentShader)
{
	_id = glCreateProgram();
	glAttachShader(_id, vertexShader);
	glAttachShader(_id, fragmentShader);
	glLinkProgram(_id);

	GLint linkStatus;
	GLchar programLinkInfo[512];
	glGetProgramiv(_id, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		glGetProgramInfoLog(_id, 512, NULL, programLinkInfo);
		cout << "ERROR: SHADER PROGRAM LINKING FAILED\n" << programLinkInfo << "\n";
		_valid = false;
	}
	else
		cout << "SUCCESS: SHADER PROGRAM LINKING SUCCESSFUL\n";
}