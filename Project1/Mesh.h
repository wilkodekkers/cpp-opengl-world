#pragma once
#include <glm/gtc/type_ptr.hpp>
#include "glsl.h"

class mesh
{
public:
	glm::vec3 ambient_color;
	glm::vec3 diffuse_color;
	glm::vec3 specular_color;
	float power;

	mesh();
	mesh(glm::vec3 ambient_color, glm::vec3 diffuse_color, glm::vec3 specular_color, float power);

	void fill_uniform_vars(GLuint uniform_material_ambient, GLuint uniform_material_diffuse, GLuint uniform_specular, GLuint uniform_material_power);
};
