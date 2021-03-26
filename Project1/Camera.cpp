#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera()
{
}

Camera::Camera(int width, int height)
{
	m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	view = glm::lookAt(
		m_CameraPos,
		m_CameraPos + m_CameraFront,
		m_CameraUp);
	projection = glm::perspective(
		glm::radians(45.4f),
		1.0f * width / height,
		0.1f,
		20.0f);
}

glm::mat4 Camera::getView()
{
	return glm::lookAt(
		m_CameraPos,
		m_CameraPos + m_CameraFront,
		m_CameraUp);
}

glm::mat4 Camera::getProjection()
{
	return projection;
}
