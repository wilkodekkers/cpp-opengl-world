#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

camera::camera() = default;

camera::camera(const int width, const int height)
{
	// Initialize camera variables
	position_ = glm::vec3(0.0f, 1.75f, 5.0f);
	front_ = glm::vec3(0.0f, 0.0f, -1.0f);
	up_ = glm::vec3(0.0f, 1.0f, 0.0f);

	// Setting up the camera view
	view_ = glm::lookAt(
		position_,
		position_ + front_,
		up_);

	// Setting up the camera perspective
	projection_ = glm::perspective(
		glm::radians(45.4f),
		1.0f * static_cast<float>(width) / static_cast<float>(height),
		0.1f,
		20.0f);
}

camera::camera(const int width, const int height, const glm::vec3& position, const glm::vec3& front, const glm::vec3& up)
{
	// Initialize camera variables
	this->position_ = position;
	this->front_ = front;
	this->up_ = up;

	// Setting up the camera view
	view_ = get_view();

	// Setting up the camera perspective
	projection_ = glm::perspective(
		glm::radians(45.4f),
		1.0f * static_cast<float>(width) / static_cast<float>(height),
		0.1f,
		20.0f);
}

glm::mat4 camera::get_view() const
{
	return lookAt(
		position_,
		position_ + front_,
		up_);
}
