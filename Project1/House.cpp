#include "house.h"

house::house()
{
	// Default house position
	pos_ = glm::vec3(0.0f, 0.0f, 0.0f);
}

house::house(const object base, const object roof, const object car, const object street_light, const glm::vec3 pos)
{
	// Initialize house variables
	this->base_ = base;
	this->roof_ = roof;
	this->car_ = car;
	this->street_light_ = street_light;
	this->pos_ = pos;
}

house::~house()
{
}

void house::init_model()
{
	// Init all house related models
	base_.init_model();
	roof_.init_model();
	car_.init_model();
	street_light_.init_model();
}

void house::init_texture()
{
	// Init all house related textures
	base_.init_texture();
	roof_.init_texture();
	car_.init_texture();
	street_light_.init_texture();
}

void house::init_buffers(const GLuint program_id)
{
	// Init all house related buffers
	base_.init_buffers(program_id);
	roof_.init_buffers(program_id);
	car_.init_buffers(program_id);
	street_light_.init_buffers(program_id);
}

void house::init_matrices(const glm::mat4 view)
{
	// Init all house related matrices
	base_.init_matrices(view);
	roof_.init_matrices(view);
	car_.init_matrices(view);
	street_light_.init_matrices(view);

	// Move the base of the house
	base_.move(pos_);

	// Move the roof and scale/rotate it
	roof_.move(pos_ + glm::vec3(0.25, 0.65, 0.0));
	roof_.scale(glm::vec3(0.72, 1.0, 1.0));
	roof_.rotate(0.78, glm::vec3(0.0, 0.0, 1.0));

	// Move the car to the correct place and fix the scale
	car_.move(pos_ + glm::vec3(1.0, 0.0, 0.0));
	car_.scale(glm::vec3(0.05, 0.05, 0.05));

	// Move the street light to the correct place and fix the scale
	street_light_.move(pos_ + glm::vec3(0.0f, 0.0f, -1.0f));
	street_light_.scale(glm::vec3(0.1f, 0.1f, 0.1f));
	street_light_.rotate(1.6f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void house::render(const glm::mat4 view, const GLuint uniform_mv, const GLuint uniform_material_ambient, const GLuint uniform_material_diffuse, const GLuint uniform_specular, const GLuint uniform_material_power)
{
	// Roof up and down animation
	if (pos_.y < 1) {
		pos_.y += 0.001f;
		roof_.move(glm::vec3(0.001f, 0.001f, 0.0f));
	}
	else {
		pos_.y = 0.0f;
		roof_.move(glm::vec3(-1.0f, -1.0f, 0.0f));
	}

	// Render all house related objects
	base_.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
	roof_.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
	car_.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
	street_light_.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
}
