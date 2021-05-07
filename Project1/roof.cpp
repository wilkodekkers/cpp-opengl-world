#include "roof.h"

roof::roof(const char* texture_path) : object("Objects/roof.obj", texture_path, glm::vec3(1.0f, 1.0f, 1.0f))
{
}

roof::~roof() = default;
