#include "House.h"

House::House()
{
	// Default house position
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
}

House::House(Object base, Object roof, Object car, Object streetLight,  glm::vec3 pos)
{
	// Initialize house variables
	this->base = base;
	this->roof = roof;
	this->car = car;
	this->streetLight = streetLight;
	this->pos = pos;
}

House::~House()
{
}

void House::initModel()
{
	// Init all house related models
	base.initModel();
	roof.initModel();
	car.initModel();
	streetLight.initModel();
}

void House::initTexture()
{
	// Init all house related textures
	base.initTexture();
	roof.initTexture();
	car.initTexture();
	streetLight.initTexture();
}

void House::initBuffers(GLuint program_id)
{
	// Init all house related buffers
	base.initBuffers(program_id);
	roof.initBuffers(program_id);
	car.initBuffers(program_id);
	streetLight.initBuffers(program_id);
}

void House::initMatrices(glm::mat4 view)
{
	// Init all house related matrices
	base.initMatrices(view);
	roof.initMatrices(view);
	car.initMatrices(view);
	streetLight.initMatrices(view);

	// Move the base of the house
	base.move(pos);

	// Move the roof and scale/rotate it
	roof.move(pos + glm::vec3(0.25, 0.65, 0.0));
	roof.scale(glm::vec3(0.72, 1.0, 1.0));
	roof.rotate(0.78, glm::vec3(0.0, 0.0, 1.0));

	// Move the car to the correct place and fix the scale
	car.move(pos + glm::vec3(1.0, 0.0, 0.0));
	car.scale(glm::vec3(0.05, 0.05, 0.05));

	// Move the street light to the correct place and fix the scale
	streetLight.move(pos + glm::vec3(0.0f, 0.0f, -1.0f));
	streetLight.scale(glm::vec3(0.1f, 0.1f, 0.1f));
	streetLight.rotate(1.6f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void House::render(glm::mat4 view, GLuint uniform_mv, GLuint uniform_material_ambient, GLuint uniform_material_diffuse, GLuint uniform_specular, GLuint uniform_material_power)
{
	// Roof up and down animation
	if (pos.y < 1) {
		pos.y += 0.001f;
		roof.move(glm::vec3(0.001f, 0.001f, 0.0f));
	}
	else {
		pos.y = 0.0f;
		roof.move(glm::vec3(-1.0f, -1.0f, 0.0f));
	}

	// Render all house related objects
	base.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
	roof.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
	car.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
	streetLight.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
}
