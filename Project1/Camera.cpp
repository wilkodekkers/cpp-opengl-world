#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
}

Camera::Camera(int width, int height)
{
	this->view = glm::lookAt(
		glm::vec3(2.0, 2.0, 10.0),  // eye
		glm::vec3(4.0, 0.0, 0.0),  // center
		glm::vec3(0.0, 1.0, 0.0));  // up
	this->projection = glm::perspective(
		glm::radians(45.4f),
		1.0f * width / height,
		0.1f,
		20.0f);
}

glm::mat4 Camera::getView()
{
	return this->view;
}

glm::mat4 Camera::getProjection()
{
	return this->projection;
}
