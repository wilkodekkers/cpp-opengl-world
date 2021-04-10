#include "House.h"

House::House()
{
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
}

House::House(Object base, Object roof, Object car, glm::vec3 pos)
{
	this->base = base;
	this->roof = roof;
	this->car = car;
	this->pos = pos;
}

House::~House()
{
}

void House::initModel()
{
	base.initTexture();
	roof.initTexture();
	car.initTexture();
}

void House::initTexture()
{
	base.initTexture();
	roof.initTexture();
	car.initTexture();
}

void House::initBuffers(GLuint program_id)
{
	base.initBuffers(program_id);
	roof.initBuffers(program_id);
	car.initBuffers(program_id);
}

void House::initMatrices(glm::mat4 view)
{
	base.initMatrices(view);
	roof.initMatrices(view);
	car.initMatrices(view);

	base.move(pos);

	roof.move(pos + glm::vec3(0.25, 0.65, 0.0));
	roof.scale(glm::vec3(0.72, 1.0, 1.0));
	roof.rotate(0.78, glm::vec3(0.0, 0.0, 1.0));

	car.move(pos + glm::vec3(1.0, 0.0, 0.0));
	car.scale(glm::vec3(0.05, 0.05, 0.05));
}

void House::render(glm::mat4 view, GLuint uniform_mv, GLuint uniform_material_ambient, GLuint uniform_material_diffuse, GLuint uniform_specular, GLuint uniform_material_power)
{
	base.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
	roof.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
	car.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
}
