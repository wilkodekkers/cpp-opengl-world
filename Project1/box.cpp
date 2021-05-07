#include "box.h"

box::box(const char* texture_path) : object("Objects/box.obj", texture_path, glm::vec3(1.0f, 1.0f, 1.0f))
{
}

box::~box() = default;
