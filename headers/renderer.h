#pragma once

#include <GLObjects/VAO.h>
#include <GLObjects/VBO.h>
#include <GLObjects/EBO.h>
#include <GLObjects/shader.h>

class Renderer
{
public:
	void setClearColor(GLfloat clearColorR, GLfloat clearColorG, GLfloat clearColorB);

	void clear() const;

	void drawVertices(Shader& shader, VBO& vbo, VAO& vao) const;
	
	void drawElements(Shader& shader, VBO& vbo, EBO& ebo, VAO& vao) const;

private:
	GLfloat clearColor[3];
};