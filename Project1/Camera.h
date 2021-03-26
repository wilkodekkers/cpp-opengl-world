#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

class Camera
{
private:
	glm::mat4 view;
	glm::mat4 projection;
public:
	Camera();
	Camera(int width, int height);
	glm::mat4 getView();
	glm::mat4 getProjection();
};

