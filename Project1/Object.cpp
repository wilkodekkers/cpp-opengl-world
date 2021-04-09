#include "Object.h"

Object::Object()
{
	this->normals = vector<glm::vec3>();
	this->vertices = vector<glm::vec3>();
	this->uvs = vector<glm::vec2>();
	this->mesh = Mesh(
		glm::vec3(0.0f, 0.0f, 0.0f), // Ambient
		glm::vec3(0.5f, 0.5f, 0.3f), // Diffuse
		glm::vec3(1.0f, 1.0f, 1.0f), // Specular
		1024);
}

Object::Object(const char* modelPath, const char* texturePath)
{
	this->normals = vector<glm::vec3>();
	this->vertices = vector<glm::vec3>();
	this->uvs = vector<glm::vec2>();
	this->mesh = Mesh(
		glm::vec3(0.0f, 0.0f, 0.0f), // Ambient
		glm::vec3(0.5f, 0.5f, 0.3f), // Diffuse
		glm::vec3(1.0f, 1.0f, 1.0f), // Specular
		1024);
	this->modelPath = modelPath;
	this->texturePath = texturePath;
}

Object::Object(const char* modelPath, const char* texturePath, Mesh mesh)
{
	this->modelPath = modelPath;
	this->texturePath = texturePath;
	this->mesh = mesh;
}

void Object::move(glm::vec3 pos)
{
	this->model = glm::translate(this->model, pos);
}

void Object::rotate(float radians, glm::vec3 angles)
{
	model = glm::rotate(model, radians, angles);
}

void Object::scale(glm::vec3 scales)
{
	model = glm::scale(model, scales);
}

void Object::initModel()
{
	loadOBJ(this->modelPath, this->vertices, this->uvs, this->normals);
}

void Object::initTexture()
{
	if (strstr(this->texturePath, ".bmp")) {
		this->texture_id = loadBMP(this->texturePath);
	}
	else {
		this->texture_id = loadDDS(this->texturePath);
	}
}

void Object::initBuffers(GLuint program_id)
{
	// Get vertex attributes
	position_id = glGetAttribLocation(program_id, "position");
	normal_id = glGetAttribLocation(program_id, "normal");
	uv_id = glGetAttribLocation(program_id, "uv");

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
	glGenVertexArrays(1, &this->vao);

	// Bind to vao
	glBindVertexArray(this->vao);

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

void Object::initMatrices(glm::mat4 view)
{
	this->model = glm::mat4();
	this->mv = view * this->model;
}

void Object::render(glm::mat4 view, GLuint uniform_mv, GLuint uniform_material_ambient, GLuint uniform_material_diffuse, GLuint uniform_specular, GLuint uniform_material_power)
{
	glBindTexture(GL_TEXTURE_2D, this->texture_id);

	// Do transformation
	//this->model = glm::rotate(this->model, 0.01f, glm::vec3(1.0f, 0.5f, 0.2f));
	this->mv = view * this->model;

	// Send mv
	glUniformMatrix4fv(uniform_mv, 1, GL_FALSE, glm::value_ptr(this->mv));

	// Fill uniform vars for material
	this->mesh.fillUniformVars(uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);

	// Send vao
	glBindVertexArray(this->vao);
	glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());
	glBindVertexArray(0);
}
