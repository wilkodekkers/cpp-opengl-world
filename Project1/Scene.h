#pragma once
#include <GL/glew.h>
#include "Object.h"
#include "House.h"
#include "camera.h"

class scene
{
	Object sign_;
	Object floor_;
	House house_[8];
	Object car_;
	glm::vec3 light_position_;
	
	GLuint program_id_;
	GLuint uniform_mv_;
	GLuint uniform_material_ambient_;
	GLuint uniform_material_diffuse_;
	GLuint uniform_specular_;
	GLuint uniform_material_power_;
	
	float camera_speed_ = 0.05f;
	
public:
	camera m_camera;
	camera m_drone_camera;
	int camera_mode = 0;

	scene();
	~scene();
	void render();
	void init(const char* fragment, const char* vertex, int width, int height);
	void init_camera(int width, int height);
	void init_buffers();
	void init_shaders(const char* fragment, const char* vertex);
	void switch_camera();
};
