#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Object.h"
#include "Camera.h"

class Scene
{
private:
	Camera camera;
	Object objects[20];
	glm::vec3 light_position;
	
	GLuint program_id;
	GLuint uniform_mv;
	GLuint uniform_material_ambient;
	GLuint uniform_material_diffuse;
	GLuint uniform_specular;
	GLuint uniform_material_power;
public:
	Scene();
	void render();
	void init(const char* fragment, const char* vertex, int width, int height);
	void initCamera(int width, int height);
	void initBuffers();
	void initShaders(const char* fragment, const char* vertex);
};

