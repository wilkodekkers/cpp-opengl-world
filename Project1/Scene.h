#pragma once
#include <GL/glew.h>
#include "Object.h"
#include "house.h"
#include "camera.h"
#include "car.h"
#include "plane.h"
#include "sign.h"

class scene
{
	std::vector<object*> objects_;
	
	sign sign_;
	plane floor_;
	plane road_;
	house house_[8];
	car car_;
	glm::vec3 light_position_;
	
	GLuint program_id_;
	
	float camera_speed_ = 0.05f;
	camera m_camera_;
	camera m_drone_camera_;
	
public:
	
	int camera_mode = 0;

	scene();
	~scene();
	void render();
	void init(const char* fragment, const char* vertex, int width, int height);
	void init_camera(int width, int height);
	void init_buffers();
	void init_shaders(const char* fragment, const char* vertex);
	void switch_camera();
	camera* get_camera() { return camera_mode == 0 ? &m_camera_ : &m_drone_camera_; }
};
