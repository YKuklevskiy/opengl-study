#pragma once

#include <vector>
#include <GLObjects/GLObjects.h>

class Mesh {
public:
    // mesh data
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture*> textures);
    ~Mesh();

    void Draw(Shader& shader);

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;

    static VertexAttributeLayout* attributeLayout;

    VAO vao;
    VBO vbo;
    EBO ebo;

    void setupMesh();
};