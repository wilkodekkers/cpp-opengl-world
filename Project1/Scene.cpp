#include "Scene.h"
#include "box.h"
#include "car.h"
#include "plane.h"
#include "roof.h"
#include "sign.h"
#include "street_light.h"

scene::scene() 
{
	// Init scene variables to remove warnings
	program_id_ = GLuint();
}

scene::~scene() = default;

void scene::render()
{
	// Attach to program_id
	glUseProgram(this->program_id_);

	car_.rotate(0.03f, glm::vec3(0.0f, 1.0f, 0.0f));
	sign_.rotate(0.01f, glm::vec3(1.0f, 0.0f, 0.0f));

	const glm::mat4 view = get_camera()->get_view();
	
	floor_.render(view);
	road_.render(view);
	sign_.render(view);
	car_.render(view);

	// Render houses
	for (auto& i : house_)
	{
		i.render(view);
	}
}

void scene::init(const char* fragment, const char* vertex, const int width, const int height)
{
	// init base classes
	const auto base = box("Textures/house_bricks.bmp");
	const auto m_roof = roof("Textures/roof_panes.bmp");
	const auto m_car = car();
	const auto m_street_light = street_light();
	
	// Create street of houses
	for (int i = 0; i < 4; i++) {
		house_[i] = house(base, m_roof, m_car, m_street_light, glm::vec3(2.0 * i, 0.0, 0.0));
	}
	for (int i = 4; i < 8; i++) {
		house_[i] = house(base, m_roof, m_car, m_street_light, glm::vec3(2.0 * (i - 4), 0.0, 4.0));
	}

	// Set variables
	this->car_ = m_car;
	this->floor_ = plane("Textures/grass.bmp");
	this->road_ = plane("Textures/road.bmp");
	this->sign_ = sign();
	this->light_position_ = glm::vec3(4.0f, 4.0f, 4.0f);

	// Init other scene methods
	this->init_shaders(fragment, vertex);
	this->init_camera(width, height);
	this->init_buffers();
}

void scene::init_camera(int width, int height)
{
	// Set camera variables
	this->m_camera_ = camera(width, height);
	this->m_drone_camera_ = camera(width, height, glm::vec3(0.0f, 10.0f, 5.0f), glm::vec3(0.0f, -2.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Set house matrices
	for (auto& i : house_)
	{
		i.init_matrices(this->m_camera_.get_view());
	}

	// Set other scene matrices
	floor_.init_matrices(this->m_camera_.get_view());
	car_.init_matrices(this->m_camera_.get_view());
	sign_.init_matrices(this->m_camera_.get_view());
	road_.init_matrices(m_camera_.get_view());

	// Move the floor and scale it correctly
	floor_.move(glm::vec3(2.5f, 0.0f, 2.5f));
	floor_.scale(glm::vec3(10.0f, 0.01f, 10.0f));

	// Move the road and scale it correctly
	road_.move(glm::vec3(2.5f, 0.01f, 2.0f));
	road_.scale(glm::vec3(10.0f, 0.01f, 2.0f));
	road_.rotate(1.57079633f, glm::vec3(0.0f, 1.0f, 0.0f));

	// Move the car and scale it correctly
	car_.move(glm::vec3(2.0, 0.0, 2.0));
	car_.scale(glm::vec3(0.05, 0.05, 0.05));
	car_.rotate(1.6f, glm::vec3(0.0, 1.0, 0.0));

	// Move the sign and scale it correctly
	sign_.move(glm::vec3(-2.0f, 0.0f, 3.0f));
	sign_.scale(glm::vec3(0.1f, 0.1f, 0.1f));
	sign_.rotate(1.6f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void scene::init_buffers()
{
	for (auto& i : house_)
	{
		i.init_buffers(program_id_);
	}
	floor_.init_buffers(program_id_);
	car_.init_buffers(program_id_);
	sign_.init_buffers(program_id_);
	road_.init_buffers(program_id_);

	// Make uniform vars
	const GLuint uniform_proj = glGetUniformLocation(program_id_, "projection");
	const GLuint uniform_light_pos = glGetUniformLocation(program_id_, "light_pos");

	// Attach to program
	glUseProgram(this->program_id_);

	// Fill uniform vars
	glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, glm::value_ptr(m_camera_.get_projection()));
	glUniform3fv(uniform_light_pos, 1, glm::value_ptr(light_position_));
}

void scene::init_shaders(const char* fragment, const char* vertex)
{
	char* vertexshader = glsl::read_file(vertex);
	const GLuint vsh_id = glsl::make_vertex_shader(vertexshader);

	char* fragshader = glsl::read_file(fragment);
	const GLuint fsh_id = glsl::make_fragment_shader(fragshader);

	program_id_ = glsl::make_shader_program(vsh_id, fsh_id);

	car_.init_shader(fragment, vertex, program_id_);
}

void scene::switch_camera()
{
	if (camera_mode == 0)
		camera_mode = 1;
	else
		camera_mode = 0;
}
