#include "House.h"

House::House()
{
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
}

House::House(Object base, Object roof, glm::vec3 pos)
{
	this->base = base;
	this->roof = roof;
	this->pos = pos;
}

House::~House()
{
}

void House::initModel()
{
	base.initTexture();
	roof.initTexture();
}

void House::initTexture()
{
	base.initTexture();
	roof.initTexture();
}

void House::initBuffers(GLuint program_id)
{
	base.initBuffers(program_id);
	roof.initBuffers(program_id);
}

void House::initMatrices(glm::mat4 view)
{
	base.initMatrices(view);
	roof.initMatrices(view);

	base.move(pos);
	roof.scale(glm::vec3(1.0, 1.0, 0.7));
	roof.move(pos + glm::vec3(0.0, 0.65, 0.35));
	roof.rotate(-0.78, glm::vec3(1.0, 0.0, 0.0));
}

void House::render(glm::mat4 view, GLuint uniform_mv, GLuint uniform_material_ambient, GLuint uniform_material_diffuse, GLuint uniform_specular, GLuint uniform_material_power)
{
	base.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
	roof.render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
}
