#include "skybox.h"

#include <vector>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "texture.h"

skybox::skybox()
{
	
}

void skybox::init()
{
	sky_box_shader = shader("skybox.vs", "skybox.frag");
	GLfloat sky_box_vertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &sky_box_vao);
	glGenBuffers(1, &sky_box_vbo);
	glBindVertexArray(sky_box_vao);
	glBindBuffer(GL_ARRAY_BUFFER, sky_box_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sky_box_vertices), &sky_box_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	std::vector<const GLchar*> faces;
	faces.push_back("Textures/skybox/right.tga");
	faces.push_back("Textures/skybox/left.tga");
	faces.push_back("Textures/skybox/top.tga");
	faces.push_back("Textures/skybox/bottom.tga");
	faces.push_back("Textures/skybox/back.tga");
	faces.push_back("Textures/skybox/front.tga");

	cube_map_texture = load_cube_map(faces);
}

void skybox::render(const glm::mat4& view, const glm::mat4& projection) const
{
	glDepthFunc(GL_LEQUAL);
	
	sky_box_shader.use();
	
	glUniformMatrix4fv(glGetUniformLocation(sky_box_shader.get_program(), "view"), 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(sky_box_shader.get_program(), "projection"), 1, GL_FALSE, value_ptr(projection));
	
	glBindVertexArray(sky_box_vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	
	glDepthFunc(GL_LESS);
}
