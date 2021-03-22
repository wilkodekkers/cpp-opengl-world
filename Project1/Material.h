#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glsl.h"


class Material
{
public:
	glm::vec3 ambient_color;
	glm::vec3 diffuse_color;
	glm::vec3 specular_color;
	float power;

	Material();
	Material(glm::vec3 ambient_color, glm::vec3 diffuse_color, glm::vec3 specular_color, float power);

	void fillUniformVars(GLuint uniform_material_ambient, GLuint uniform_material_diffuse, GLuint uniform_specular, GLuint uniform_material_power);
};

