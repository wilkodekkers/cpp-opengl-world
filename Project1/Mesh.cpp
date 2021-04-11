#include "Mesh.h"

Mesh::Mesh()
{
	// Initialize mesh variables
	this->ambient_color = glm::vec3();
	this->diffuse_color = glm::vec3();
	this->specular_color = glm::vec3();
	this->power = 0;
}

Mesh::Mesh(glm::vec3 ambient_color, glm::vec3 diffuse_color, glm::vec3 specular_color, float power)
{
	// Initialize mesh variables with custom values
	this->ambient_color = ambient_color;
	this->diffuse_color = diffuse_color;
	this->specular_color = specular_color;
	this->power = power;
}

void Mesh::fillUniformVars(GLuint uniform_material_ambient, GLuint uniform_material_diffuse, GLuint uniform_specular, GLuint uniform_material_power)
{
	// Fill uniform vars for mesh
	glUniform3fv(uniform_material_ambient, 1, glm::value_ptr(this->ambient_color));
	glUniform3fv(uniform_material_diffuse, 1, glm::value_ptr(this->diffuse_color));
	glUniform3fv(uniform_specular, 1, glm::value_ptr(this->specular_color));
	glUniform1f(uniform_material_power, this->power);
}
