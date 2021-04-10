#pragma once

#include "Object.h"

class House
{
private:
	Object base;
	Object roof;
	Object car;
	glm::vec3 pos;
public:
	House();
	House(Object base, Object roof, Object car, glm::vec3 pos);
	~House();

	void initModel();
	void initTexture();
	void initBuffers(GLuint program_id);
	void initMatrices(glm::mat4 view);
	void render(glm::mat4 view, GLuint uniform_mv, GLuint uniform_material_ambient, GLuint uniform_material_diffuse, GLuint uniform_specular, GLuint uniform_material_power);
};

