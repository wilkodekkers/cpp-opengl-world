#include "sign.h"

sign::sign() : object("Objects/sign.obj", "Textures/sign.bmp", glm::vec3(100.0f, 100.0f, 100.0f))
{
}

sign::~sign() = default;

void sign::animate()
{
	rotate(0.01f, glm::vec3(1.0f, 0.0f, 0.0f));
}
