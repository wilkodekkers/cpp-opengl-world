#pragma once
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "glsl.h"
#include "mesh.h"

using namespace std;

class object
{
public:
	vector<glm::vec3> normals;
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;

	glm::mat4 model;
	glm::mat4 mv;

	GLuint vao;
	
	GLuint position_id;
	GLuint normal_id;
	GLuint uv_id;
	GLuint texture_id;

	GLuint vbo_vertices;
	GLuint vbo_normals;
	GLuint vbo_uvs;

	mesh m_mesh;

	const char* model_path;
	const char* texture_path;

	object();
	object(const char* model_path, const char* texture_path, glm::vec3 specular);
	object(const char* model_path, const char* texture_path, mesh m_mesh);

	void move(glm::vec3 pos);
	void rotate(float radians, glm::vec3 angles);
	void scale(glm::vec3 scales);

	void init_model();
	void init_texture();
	void init_shaders(const char* fragment, const char* vertex, GLuint program_id);
	void init_buffers(GLuint program_id);
	void init_matrices(glm::mat4 view);
	void render(glm::mat4 view, GLuint uniform_mv, GLuint uniform_material_ambient, GLuint uniform_material_diffuse, GLuint uniform_specular, GLuint uniform_material_power);
};

