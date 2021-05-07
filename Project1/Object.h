#pragma once
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "glsl.h"
#include "mesh.h"

using namespace std;

class object
{
protected:
	GLuint vao_;
	GLuint uniform_mv_;
	GLuint uniform_material_ambient_;
	GLuint uniform_material_diffuse_;
	GLuint uniform_specular_;
	GLuint uniform_material_power_;

	glm::vec3 position_;
public:
	vector<glm::vec3> normals;
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;

	glm::mat4 model;
	glm::mat4 mv;
	
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

	void move(const glm::vec3& pos);
	void rotate(float radians, glm::vec3 angles);
	void scale(glm::vec3 scales);
	virtual void animate();
	
	void init_model();
	void init_texture();
	void init_shader(const char* fragment, const char* vertex, GLuint program_id);
	void init_buffers(GLuint program_id);
	void init_matrices(glm::mat4 view);
	void render(glm::mat4 view);

	glm::vec3 get_position() const { return position_; }
};

