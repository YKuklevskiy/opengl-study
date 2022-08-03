#pragma once

#include <assimp/scene.h>

#include <mesh.h>

#include <vector>

class Model
{
public:
    Model(string path);
    ~Model();
    
    // Calls Draw method for all meshes in the model.
    void Draw(Shader& shader);

private:
    std::vector<Mesh*> meshes;
    string directory;
    std::vector<string> loadedTextures;
    bool isValid = true;

    // loads a model from a file using AssImp, then parses the data into meshes vector.
    void loadModel(string path);

    // Parses all meshes of a node and pushes them to meshes. 
    // Recursively processes all the nodes in the model tree.
    void processNode(aiNode* node, const aiScene* scene);
    
    // Parses aiMesh type object into Mesh type object.
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

    //
    //      Helper functions for parsing aiMesh into own Mesh object
    //

    // Parses material from aiMesh into texture vector.
    std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type);

    // Parses i-th vertex's data from mesh, and puts the data into resultVertex.
    void processMeshVertex(aiMesh* const mesh, unsigned int vertexIndex, Vertex& resultVertex);

    // Parses all data from faces of mesh into indices vector.
    void processMeshIndices(aiMesh* const mesh, std::vector<unsigned int>& indices);
};
