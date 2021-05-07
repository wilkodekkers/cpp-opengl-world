#pragma once
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

class skybox
{
	GLuint sky_box_vao;
	GLuint sky_box_vbo;
	GLuint cube_map_texture;
	Shader sky_box_shader;
public:
	skybox();
	~skybox() = default;

	void init();
	void render(const glm::mat4& view, const glm::mat4& projection);
};

