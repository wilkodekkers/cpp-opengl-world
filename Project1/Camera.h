#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera
{
private:
	glm::mat4 view;
	glm::mat4 projection;
public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	Camera();
	Camera(int width, int height);
	Camera(int width, int height, glm::vec3 position, glm::vec3 front, glm::vec3 up);
	glm::mat4 getView();
	glm::mat4 getProjection();
};

