#include "plane.h"

plane::plane(const char* texture_path) : object("Objects/box.obj", texture_path, glm::vec3(1.0f, 1.0f, 1.0f))
{
}

plane::~plane() = default;
