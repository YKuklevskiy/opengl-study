#include <renderer.h>

void Renderer::setClearColor(GLfloat clearColorR, GLfloat clearColorG, GLfloat clearColorB)
{
	clearColor[0] = clearColorR;
	clearColor[1] = clearColorG;
	clearColor[2] = clearColorB;
	
	glClearColor(clearColor[0], clearColor[1], clearColor[2], 1.0f);
}

void Renderer::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::drawVertices(Shader& shader, VBO& vbo, VAO& vao) const
{
	vao.bind();
	vbo.bind();
	shader.use();

	GLuint size = vbo.getBufferSize() / vao.getLayoutSize();

	glDrawArrays(GL_TRIANGLES, 0, size);
}

void Renderer::drawElements(Shader& shader, VBO& vbo, EBO& ebo, VAO& vao) const
{
	vao.bind();
	vbo.bind();
	ebo.bind();
	shader.use();

	GLuint size = ebo.getBufferSize();

	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}