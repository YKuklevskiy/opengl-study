#include <material.h>

Material::Material(Shader* const shader)
{
	this->shader = shader;
	ambient = { 0.0f, 0.0f, 0.0f };
	diffuse = { 0.0f, 0.0f, 0.0f };
	specular = { 0.0f, 0.0f, 0.0f };
	shininess = 0.0f;
}

void Material::setUniforms(std::string shaderStructName) const
{
	shader->setVec3f(shaderStructName + ".ambient", ambient);
	shader->setVec3f(shaderStructName + ".diffuse", diffuse);
	shader->setVec3f(shaderStructName + ".specular", specular);
	shader->setFloat(shaderStructName + ".shininess", shininess);
}