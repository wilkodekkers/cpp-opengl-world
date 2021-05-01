#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

camera::camera() = default;

camera::camera(const int width, const int height)
{
	// Initialize camera variables
	position = glm::vec3(0.0f, 1.75f, 5.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Setting up the camera view
	view_ = glm::lookAt(
		position,
		position + front,
		up);

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
	this->position = position;
	this->front = front;
	this->up = up;

	// Setting up the camera view
	view_ = glm::lookAt(
		position,
		position + front,
		up);

	// Setting up the camera perspective
	projection_ = glm::perspective(
		glm::radians(45.4f),
		1.0f * static_cast<float>(width) / static_cast<float>(height),
		0.1f,
		20.0f);
}

glm::mat4 camera::get_view() const
{
	return glm::lookAt(
		position,
		position + front,
		up);
}

glm::mat4 camera::get_projection() const
{
	return projection_;
}
