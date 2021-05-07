#include "Object.h"
#include <glm/gtx/transform.hpp>

#include "objloader.h"
#include "texture.h"

object::object()
{
	// Initialize object variables
	this->normals = vector<glm::vec3>();
	this->vertices = vector<glm::vec3>();
	this->uvs = vector<glm::vec2>();
	this->m_mesh = mesh(
		glm::vec3(0.0f, 0.0f, 0.0f), // Ambient
		glm::vec3(0.5f, 0.5f, 0.3f), // Diffuse
		glm::vec3(1.0f, 1.0f, 1.0f), // Specular
		1024);
	this->position_ = glm::vec3(0.0f, 0.0f, 0.0f);
}

object::object(const char* model_path, const char* texture_path, const glm::vec3 specular)
{
	// Initialize object variables with custom values
	this->normals = vector<glm::vec3>();
	this->vertices = vector<glm::vec3>();
	this->uvs = vector<glm::vec2>();
	this->m_mesh = mesh(
		glm::vec3(0.0f, 0.0f, 0.0f), // Ambient
		glm::vec3(0.5f, 0.5f, 0.3f), // Diffuse
		specular, // Specular
		1024);
	this->model_path = model_path;
	this->texture_path = texture_path;

	init_model();
	init_texture();
}

object::object(const char* model_path, const char* texture_path, const mesh mesh)
{
	// Initialize object variables with custom mesh
	this->model_path = model_path;
	this->texture_path = texture_path;
	this->m_mesh = mesh;

	init_model();
	init_texture();
}

void object::move(const glm::vec3& pos)
{
	this->model = translate(this->model, pos);
	position_ = position_ + pos;
}

void object::rotate(const float radians, const glm::vec3 angles)
{
	// Rotate the object
	model = glm::rotate(model, radians, angles);
}

void object::scale(glm::vec3 scales)
{
	// Scale the model
	model = glm::scale(model, scales);
}

void object::animate()
{
}

void object::init_model()
{
	// Init the model
	loadOBJ(this->model_path, this->vertices, this->uvs, this->normals);
}

void object::init_texture()
{
	// Init the texture base on the extension (only works for bmp and dds)
	if (strstr(this->texture_path, ".bmp")) {
		this->texture_id = loadBMP(this->texture_path);
	}
	else {
		this->texture_id = loadDDS(this->texture_path);
	}
}

void object::init_shader(const char* fragment, const char* vertex, GLuint program_id)
{
	// Init the shader
	char* vertexshader = glsl::read_file(vertex);
	GLuint vsh_id = glsl::make_vertex_shader(vertexshader);

	char* fragshader = glsl::read_file(fragment);
	GLuint fsh_id = glsl::make_fragment_shader(fragshader);

	program_id = glsl::make_shader_program(vsh_id, fsh_id);
}

void object::init_buffers(GLuint program_id)
{
	// Get vertex attributes
	position_id = glGetAttribLocation(program_id, "position");
	normal_id = glGetAttribLocation(program_id, "normal");
	uv_id = glGetAttribLocation(program_id, "uv");

	uniform_mv_ = glGetUniformLocation(program_id, "mv");
	uniform_material_ambient_ = glGetUniformLocation(program_id, "mat_ambient");
	uniform_material_diffuse_ = glGetUniformLocation(program_id, "mat_diffuse");
	uniform_specular_ = glGetUniformLocation(program_id, "mat_specular");
	uniform_material_power_ = glGetUniformLocation(program_id, "mat_power");

	// vbo for vertices
	glGenBuffers(1, &this->vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for normals
	glGenBuffers(1, &vbo_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for uvs
	glGenBuffers(1, &vbo_uvs);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Allocate memory for vao
	glGenVertexArrays(1, &this->vao_);

	// Bind to vao
	glBindVertexArray(this->vao_);

	// Bind vertices to vao
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glVertexAttribPointer(position_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Bind normals to vao
	glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
	glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normal_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Bind uvs to vao
	glBindBuffer(GL_ARRAY_BUFFER, vbo_uvs);
	glVertexAttribPointer(uv_id, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(uv_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Stop bind to vao
	glBindVertexArray(0);
}

void object::init_matrices(const glm::mat4 view)
{
	// Init the matrices
	this->model = glm::mat4();
	this->mv = view * this->model;
}

void object::render(const glm::mat4 view)
{
	glBindTexture(GL_TEXTURE_2D, this->texture_id);

	// Do transformation
	//this->model = glm::rotate(this->model, 0.01f, glm::vec3(1.0f, 0.5f, 0.2f));
	this->mv = view * this->model;

	// Send mv
	glUniformMatrix4fv(uniform_mv_, 1, GL_FALSE, glm::value_ptr(this->mv));

	// Fill uniform vars for material
	this->m_mesh.fill_uniform_vars(uniform_material_ambient_, uniform_material_diffuse_, uniform_specular_, uniform_material_power_);

	// Send vao
	glBindVertexArray(this->vao_);
	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	glBindVertexArray(0);
}
