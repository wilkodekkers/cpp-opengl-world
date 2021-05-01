#pragma once
#include <glm/glm.hpp>

class camera
{
	glm::mat4 view_;
	glm::mat4 projection_;
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	camera();
	camera(int width, int height);
	camera(int width, int height, const glm::vec3& position, const glm::vec3& front, const glm::vec3& up);
	glm::mat4 get_view() const;
	glm::mat4 get_projection() const;
};
