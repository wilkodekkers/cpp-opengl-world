#include "scene.h"
#include <GL/glut.h>

scene::scene() 
{
	// Init scene variables to remove warnings
	program_id_ = GLuint();
	uniform_mv_ = GLuint();
	uniform_material_ambient_ = GLuint();
	uniform_material_diffuse_ = GLuint();
	uniform_specular_ = GLuint();
	uniform_material_power_ = GLuint();
}

scene::~scene()
{
	
}

void scene::render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Attach to program_id
	glUseProgram(this->program_id_);

	car_.rotate(0.03f, glm::vec3(0.0f, 1.0f, 0.0f));
	sign_.rotate(0.01f, glm::vec3(1.0f, 0.0f, 0.0f));

	camera camera;
	if (camera_mode == 0)
		camera = this->m_camera;
	else
		camera = this->m_drone_camera;

	floor_.render(camera.get_view(), uniform_mv_, uniform_material_ambient_, uniform_material_diffuse_, uniform_specular_, uniform_material_power_);
	sign_.render(camera.get_view(), uniform_mv_, uniform_material_ambient_, uniform_material_diffuse_, uniform_specular_, uniform_material_power_);
	car_.render(camera.get_view(), uniform_mv_, uniform_material_ambient_, uniform_material_diffuse_, uniform_specular_, uniform_material_power_);

	// Render houses
	for (int i = 0; i < 8; i++) {
		house_[i].render(camera.get_view(), uniform_mv_, uniform_material_ambient_, uniform_material_diffuse_, uniform_specular_, uniform_material_power_);
	}

	// Swap buffers
	glutSwapBuffers();
}

void scene::init(const char* fragment, const char* vertex, int width, int height)
{
	// Init base model
	auto base = Object("Objects/box.obj", "Textures/house_bricks.bmp", glm::vec3(1.0f, 1.0f, 1.0f));
	base.initModel();
	base.initTexture();

	// Init roof model
	auto roof = Object("Objects/roof.obj", "Textures/roof_panes.bmp", glm::vec3(1.0f, 1.0f, 1.0f));
	roof.initModel();
	roof.initTexture();

	// Init car model
	auto car = Object("Objects/car.obj", "Textures/car.bmp", glm::vec3(1.0f, 1.0f, 1.0f));
	car.initModel();
	car.initTexture();

	// Init street light model
	auto streetLight = Object("Objects/street_light.obj", "Textures/street_lantern.bmp", glm::vec3(1.0f, 1.0f, 1.0f));
	streetLight.initModel();
	streetLight.initTexture();

	// Set car variable
	this->car_ = car;

	// Fill house array
	for (int i = 0; i < 4; i++) {
		house_[i] = House(base, roof, car, streetLight, glm::vec3(2.0 * i, 0.0, 0.0));
	}
	for (int i = 4; i < 8; i++) {
		house_[i] = House(base, roof, car, streetLight, glm::vec3(2.0 * (i - 4), 0.0, 4.0));
	}

	// Init floor model
	auto floor = Object("Objects/box.obj", "Textures/grass.bmp", glm::vec3(100.0f, 1.0f, 1.0f));
	floor.initModel();
	floor.initTexture();
	this->floor_ = floor;

	// Init sign model
	auto sign = Object("Objects/sign.obj", "Textures/sign.bmp", glm::vec3(100.0f, 100.0f, 100.0f));
	sign.initModel();
	sign.initTexture();
	this->sign_ = sign;

	// Init light position
	this->light_position_ = glm::vec3(4.0f, 4.0f, 4.0f);

	// Init other scene methods
	this->init_shaders(fragment, vertex);
	this->init_camera(width, height);
	this->init_buffers();
}

void scene::init_camera(int width, int height)
{
	// Set camera variables
	this->m_camera = camera(width, height);
	this->m_drone_camera = camera(width, height, glm::vec3(0.0f, 10.0f, 5.0f), glm::vec3(0.0f, -2.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Set house matrices
	for (int i = 0; i < 8; i++) {
		house_[i].initMatrices(this->m_camera.get_view());
	}

	// Set other scene matrices
	floor_.initMatrices(this->m_camera.get_view());
	car_.initMatrices(this->m_camera.get_view());
	sign_.initMatrices(this->m_camera.get_view());

	// Move the floor and scale it correctly
	floor_.move(glm::vec3(2.5f, 0.0f, 2.5f));
	floor_.scale(glm::vec3(10.0f, 0.01f, 10.f));

	// Move the car and scale it correctly
	car_.move(glm::vec3(2.0, 0.0, 2.0));
	car_.scale(glm::vec3(0.05, 0.05, 0.05));
	car_.rotate(1.6, glm::vec3(0.0, 1.0, 0.0));

	// Move the sign and scale it correctly
	sign_.move(glm::vec3(-2.0f, 0.0f, 3.0f));
	sign_.scale(glm::vec3(0.1f, 0.1f, 0.1f));
	sign_.rotate(1.6, glm::vec3(0.0f, 1.0f, 0.0f));
}

void scene::init_buffers()
{
	for (int i = 0; i < 8; i++) {
		house_[i].initBuffers(program_id_);
	}
	floor_.initBuffers(program_id_);
	car_.initBuffers(program_id_);
	sign_.initBuffers(program_id_);

	// Make uniform vars
	uniform_mv_ = glGetUniformLocation(program_id_, "mv");
	const GLuint uniform_proj = glGetUniformLocation(program_id_, "projection");
	GLuint uniform_light_pos = glGetUniformLocation(program_id_, "light_pos");
	uniform_material_ambient_ = glGetUniformLocation(program_id_, "mat_ambient");
	uniform_material_diffuse_ = glGetUniformLocation(program_id_, "mat_diffuse");
	uniform_specular_ = glGetUniformLocation(program_id_, "mat_specular");
	uniform_material_power_ = glGetUniformLocation(program_id_, "mat_power");

	// Attach to program
	glUseProgram(this->program_id_);

	// Fill uniform vars
	glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, glm::value_ptr(m_camera.get_projection()));
	glUniform3fv(uniform_light_pos, 1, glm::value_ptr(light_position_));
}

void scene::init_shaders(const char* fragment, const char* vertex)
{
	char* vertexshader = glsl::read_file(vertex);
	const GLuint vsh_id = glsl::make_vertex_shader(vertexshader);

	char* fragshader = glsl::read_file(fragment);
	const GLuint fsh_id = glsl::make_fragment_shader(fragshader);

	program_id_ = glsl::make_shader_program(vsh_id, fsh_id);

	car_.initShaders(fragment, vertex, program_id_);
}

void scene::switch_camera()
{
	if (camera_mode == 0)
		camera_mode = 1;
	else
		camera_mode = 0;
}
