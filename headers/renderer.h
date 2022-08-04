#pragma once

#include <GLObjects/GLObjects.h>

static class Renderer
{
public:
	void setClearColor(GLfloat clearColorR, GLfloat clearColorG, GLfloat clearColorB);

	void clear() const;

	static void drawVertices(Shader& shader, VBO& vbo, VAO& vao);
	
	static void drawElements(Shader& shader, VBO& vbo, EBO& ebo, VAO& vao);

private:
	GLfloat clearColor[3];
};