#include "model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <map>
#include <algorithm>

Model::Model(string path)
{
	cout << "LOADING MODEL from \"" << path << "\"...\n";

	loadModel(path);

	if(isValid)
		cout << "SUCCESS: MODEL LOADED\n";
	else
		cout << "ERROR: COULD NOT LOAD MODEL\n";
}

Model::~Model()
{
	for (int i = 0; i < meshes.size(); i++)
		delete meshes[i];
}

void Model::Draw(Shader& shader)
{
	for (auto& mesh : meshes)
		mesh->Draw(shader);
}

void Model::loadModel(string path)
{
	Assimp::Importer import;

	const unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals;
	const aiScene* scene = import.ReadFile(path, flags);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
		isValid = false;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

	loadedTextures.clear();
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	// TODO: a lot of pushes to vectors, maybe allocate enough memory for vectors before starting to fill them up with data

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		processMeshVertex(mesh, i, vertex);
		vertices.push_back(vertex);
	}

	processMeshIndices(mesh, indices);
	
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture*> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture*> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return new Mesh(vertices, indices, textures);
}

std::vector<Texture*> Model::loadMaterialTextures(aiMaterial* material, aiTextureType aiType)
{
	const std::map<aiTextureType, TextureType> aiTypeToType =
	{
		{aiTextureType_DIFFUSE, TextureType::DIFFUSE_MAP},
		{aiTextureType_SPECULAR, TextureType::SPECULAR_MAP}
	};

    std::vector<Texture*> textures;
    for(unsigned int i = 0; i < material->GetTextureCount(aiType); i++)
    {
        aiString textureFileName;
        material->GetTexture(aiType, i, &textureFileName);
		
		if (std::find(loadedTextures.begin(), loadedTextures.end(), textureFileName.C_Str()) != loadedTextures.end())
			continue;

        Texture* texture = new Texture(directory + "/" + textureFileName.C_Str(), aiTypeToType.at(aiType));

        textures.push_back(texture);
		loadedTextures.push_back(textureFileName.C_Str());
    }
    return textures;
}

void Model::processMeshVertex(aiMesh* const mesh, unsigned int vertexIndex, Vertex& resultVertex)
{
	glm::vec3 vector;

	vector.x = mesh->mVertices[vertexIndex].x;
	vector.y = mesh->mVertices[vertexIndex].y;
	vector.z = mesh->mVertices[vertexIndex].z;
	resultVertex.position = vector;

	vector.x = mesh->mNormals[vertexIndex].x;
	vector.y = mesh->mNormals[vertexIndex].y;
	vector.z = mesh->mNormals[vertexIndex].z;
	resultVertex.normal = vector;

	if (mesh->mTextureCoords[0])
	{
		vector.x = mesh->mTextureCoords[0][vertexIndex].x;
		vector.y = mesh->mTextureCoords[0][vertexIndex].y;
		resultVertex.texCoords = glm::vec2(vector);
	}
	else
	{
		resultVertex.texCoords = glm::vec2(0.0f, 0.0f);
	}
}

void Model::processMeshIndices(aiMesh* const mesh, std::vector<unsigned int>& indices)
{
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}
