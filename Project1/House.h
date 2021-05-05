#pragma once

#include "Object.h"

class house : object
{
	object base_;
	object roof_;
	object car_;
	object street_light_;
	glm::vec3 pos_;
public:
	house();
	house(object base, object roof, object car, object street_light, glm::vec3 pos);
	~house();

	void init_model();
	void init_texture();
	void init_buffers(GLuint program_id);
	void init_matrices(const glm::mat4& view);
	void render(const glm::mat4& view);
};
