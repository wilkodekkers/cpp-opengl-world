#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera
{
private:
	glm::mat4 view;
	glm::mat4 projection;
public:
	glm::vec3 m_CameraPos;
	glm::vec3 m_CameraFront;
	glm::vec3 m_CameraUp;

	Camera();
	Camera(int width, int height);
	glm::mat4 getView();
	glm::mat4 getProjection();
};

