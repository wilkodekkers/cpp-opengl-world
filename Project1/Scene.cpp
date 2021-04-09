#include "Scene.h"

Scene::Scene() {
	program_id = GLuint();
	uniform_mv = GLuint();
	uniform_material_ambient = GLuint();
	uniform_material_diffuse = GLuint();
	uniform_specular = GLuint();
	uniform_material_power = GLuint();
}

void Scene::render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Attach to program_id
	glUseProgram(this->program_id);

	for (int i = 0; i < 4; i++) {
		house[i].render(camera.getView(), uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
	}

	// Swap buffers
	glutSwapBuffers();
}

void Scene::init(const char* fragment, const char* vertex, int width, int height) {
	Object cube = Object("Objects/box.obj", "Textures/house_bricks.bmp");
	cube.initModel();
	cube.initTexture();
	
	for (int i = 0; i < 4; i++) {
		house[i] = House(cube, cube, glm::vec3(2.0 * i, 0.0, 0.0));
	}

	this->light_position = glm::vec3(4.0f, 4.0f, 4.0f);
	this->initShaders(fragment, vertex);
	this->initCamera(width, height);
	this->initBuffers();
}

void Scene::initCamera(int width, int height) {
	this->camera = Camera(width, height);
	for (int i = 0; i < 4; i++) {
		house[i].initMatrices(this->camera.getView());
	}
}

void Scene::initBuffers()
{
	for (int i = 0; i < 4; i++) {
		house[i].initBuffers(program_id);
	}

	// Make uniform vars
	uniform_mv = glGetUniformLocation(program_id, "mv");
	GLuint uniform_proj = glGetUniformLocation(program_id, "projection");
	GLuint uniform_light_pos = glGetUniformLocation(program_id, "light_pos");
	uniform_material_ambient = glGetUniformLocation(program_id, "mat_ambient");
	uniform_material_diffuse = glGetUniformLocation(program_id, "mat_diffuse");
	uniform_specular = glGetUniformLocation(program_id, "mat_specular");
	uniform_material_power = glGetUniformLocation(program_id, "mat_power");

	// Attach to program
	glUseProgram(this->program_id);

	// Fill uniform vars
	glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, glm::value_ptr(camera.getProjection()));
	glUniform3fv(uniform_light_pos, 1, glm::value_ptr(light_position));
}

void Scene::initShaders(const char* fragment, const char* vertex)
{
	char* vertexshader = glsl::readFile(vertex);
	GLuint vsh_id = glsl::makeVertexShader(vertexshader);

	char* fragshader = glsl::readFile(fragment);
	GLuint fsh_id = glsl::makeFragmentShader(fragshader);

	program_id = glsl::makeShaderProgram(vsh_id, fsh_id);
}
