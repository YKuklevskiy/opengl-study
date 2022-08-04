#include "mesh.h"

#include <renderer.h>
#include <map>

VertexAttributeLayout* Mesh::attributeLayout = nullptr;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture*> textures)
{
    if (attributeLayout == nullptr)
    {
        attributeLayout = new VertexAttributeLayout();

        // this exact order of attributes is specified by order of attributes in the Vertex struct (vertex.h)
        attributeLayout->AddAttribute<GLfloat>(3); // position
        attributeLayout->AddAttribute<GLfloat>(2); // texCoords
        attributeLayout->AddAttribute<GLfloat>(3); // normals
    }

    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    setupMesh();
}

Mesh::~Mesh()
{
    for (int i = 0; i < textures.size(); i++)
        delete textures[i];
}

void Mesh::Draw(Shader& shader)
{
    shader.use();

    const std::map<int, string> textureNames =
    { 
      { TextureType::TEXTURE,      "texture_texture" },
      { TextureType::NORMAL_MAP,   "texture_normal" },
      { TextureType::DIFFUSE_MAP,  "texture_diffuse" },
      { TextureType::SPECULAR_MAP, "texture_specular" }
    };

    std::vector<std::vector<GLint>> textureIndexArrays(4);
    
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        textures[i]->setUnit(GL_TEXTURE0 + i); // select texture unit before binding
        
        TextureType type = textures[i]->getType();

        // because of filling arrays with final texture unit indices at first, 
        // we can then make only texture_type_count uniform set calls
        textureIndexArrays[type].push_back(i);
        
        textures[i]->bind();
    }
    glActiveTexture(GL_TEXTURE0);

    for (int i = 0; i < textureIndexArrays.size(); i++)
    {
        if(textureIndexArrays[i].size() != 0)
            shader.setArrayInt("material." + textureNames.at(i), textureIndexArrays[i]);
    }

    // draw mesh
    Renderer::drawElements(shader, vbo, ebo, vao);
}

void Mesh::setupMesh()
{
    vao.bind();

    vbo.bind();
    vbo.bufferData(vertices);

    ebo.bind();
    ebo.bufferData(indices);

    vao.setupVertexAttributes(*attributeLayout);

    vao.unbind();
}
