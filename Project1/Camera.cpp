#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera()
{
}

Camera::Camera(int width, int height)
{
	// Initialize camera variables
	position = glm::vec3(0.0f, 1.75f, 5.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Setting up the camera view
	view = glm::lookAt(
		position,
		position + front,
		up);

	// Setting up the camera perspective
	projection = glm::perspective(
		glm::radians(45.4f),
		1.0f * width / height,
		0.1f,
		20.0f);
}

Camera::Camera(int width, int height, glm::vec3 position, glm::vec3 front, glm::vec3 up)
{
	// Initialize camera variables
	this->position = position;
	this->front = front;
	this->up = up;

	// Setting up the camera view
	view = glm::lookAt(
		position,
		position + front,
		up);

	// Setting up the camera perspective
	projection = glm::perspective(
		glm::radians(45.4f),
		1.0f * width / height,
		0.1f,
		20.0f);
}

glm::mat4 Camera::getView()
{
	return glm::lookAt(
		position,
		position + front,
		up);
}

glm::mat4 Camera::getProjection()
{
	return projection;
}
