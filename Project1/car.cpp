#include "car.h"

car::car() : object("Objects/car.obj", "Textures/car.bmp", glm::vec3(1.0f, 1.0f, 1.0f))
{
}

car::~car() = default;

void car::animate()
{
	rotate(0.03f, glm::vec3(0.0f, 1.0f, 0.0f));
}
