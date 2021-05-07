#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>
#include <SOIL2.h>

#include "Scene.h"
#include "Shader.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const int width = 1280, height = 960;

const char* fragshader_name = "fragmentshader.frag";
const char* vertexshader_name = "vertexshader.vert";

unsigned const int delta_time = 10;


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

scene m_scene;
GLuint sky_box_vao;
GLuint sky_box_vbo;
GLuint cube_map_texture;
Shader sky_box_shader;
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

bool is_fullscreen = false;

float last_x = width / 2;
float last_y = height / 2;
float pitch = 0.0f;
float yaw = 0.0f;

float delta_forward = 0.0f;
float delta_side = 0.0f;
float delta_vertical = 0.0f;

float move_up = 0.0f;
float move_side = 0.0f;

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void window_change_handler() {
	if (is_fullscreen) {
		glutReshapeWindow(width, height);
	}
	else {
		glutFullScreen();
		glutReshapeWindow(1920, 1080);
	}
	is_fullscreen = !is_fullscreen;
}

void keyboard_handler(const unsigned char key, int a, int b)
{
	switch (key) {
	case 27:
		glutDestroyWindow(glutGetWindow());
		break;
	case 'w':
		delta_forward = 0.1f;
		break;
	case 's':
		delta_forward = -0.1f;
		break;
	case 'a':
		delta_side = 0.1f;
		break;
	case 'd':
		delta_side = -0.1f;
		break;
	case 'i':
		move_up = 0.01f;
		break;
	case 'k':
		move_up = -0.01f;
		break;
	case 'j':
		move_side = 0.01f;
		break;
	case 'l':
		move_side = -0.01f;
		break;
	case 'v':
		m_scene.switch_camera();
		break;
	case 'q':
		delta_vertical = -0.1f;
		break;
	case 'e':
		delta_vertical = 0.1f;
		break;
	case 'f':
		window_change_handler();
		break;
	default: break;
	}
}

void keyboard_up_handler(const unsigned char key, int a, int b) {
	switch (key) {
	case 'w':
	case 's':
		delta_forward = 0.0f;
		break;
	case 'a':
	case 'd':
		delta_side = 0.0f;
		break;
	case 'i':
	case 'k':
		move_up = 0.0f;
		break;
	case 'j':
	case 'l':
		move_side = 0.0f;
		break;
	case 'q':
	case 'e':
		delta_vertical = 0.0f;
		break;
	default: break;
	}
}

//--------------------------------------------------------------------------------
// Mouse handling
//--------------------------------------------------------------------------------

void mouse_handler(const int x, const int y)
{
	// Ignore when not using the mouse
	if (x == static_cast<int>(last_x) && y == static_cast<int>(last_y)) return;
	
	float x_offset = static_cast<float>(x) - last_x;
	float y_offset = static_cast<float>(y) - last_y;

	const float sensitivity = 0.1f;
	x_offset *= sensitivity;
	y_offset *= sensitivity;

	yaw += x_offset;
	pitch -= y_offset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glutWarpPointer(width / 2, height / 2);
}

//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void handle_keyboard_camera_movement()
{
	camera* cam = m_scene.get_camera();
	
	if (delta_forward) {
		cam->set_position(cam->get_position() + delta_forward * cam->get_front());
	}
	if (delta_side) {
		cam->set_position(cam->get_position() - normalize(cross(cam->get_front(), cam->get_up())) * delta_side);
	}
	if (delta_vertical && m_scene.camera_mode == 1) {
		cam->set_position(cam->get_position() + glm::vec3(0.0f, delta_vertical, 0.0f));
	}
	if (move_side || move_up) {

		yaw -= move_side * 50;
		pitch += move_up * 50;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	if (m_scene.camera_mode == 0) {
		glm::vec3 old = cam->get_position();
		cam->set_position(glm::vec3(old.x, 0.15, old.z));
	}
}

void update_camera()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_scene.get_camera()->set_front(normalize(direction));
}

void render()
{
	glClearColor(0.2f, 0.3f, 0.8f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	handle_keyboard_camera_movement();
	update_camera();
	m_scene.render();

	glDepthFunc(GL_LEQUAL);
	sky_box_shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(sky_box_shader.Program, "view"), 1, GL_FALSE, value_ptr(m_scene.get_camera()->get_view()));
	glUniformMatrix4fv(glGetUniformLocation(sky_box_shader.Program, "projection"), 1, GL_FALSE, value_ptr(m_scene.get_camera()->get_projection()));
	glBindVertexArray(sky_box_vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map_texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

	glutSwapBuffers();
}


//------------------------------------------------------------
// void Render(int n)
// Render method that is called by the timer function
//------------------------------------------------------------

void render(int n)
{
	render();
	glutTimerFunc(delta_time, render, 0);
}


//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void init_glut_glew(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(width / 2, height / 4);
	glutCreateWindow("Wilko Dekkers | OpenGL Game");
	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard_handler);
	glutIgnoreKeyRepeat(1);
	glutKeyboardUpFunc(keyboard_up_handler);
	glutPassiveMotionFunc(mouse_handler);
	glutTimerFunc(delta_time, render, 0);
	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	glewInit();
}

//------------------------------------------------------------
// void InitScene()
// Initializes the main scene
//------------------------------------------------------------
void init_scene()
{
	m_scene = scene();
	m_scene.init(fragshader_name, vertexshader_name, GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT);
}

static GLuint LoadCubemap(vector<const GLchar* > faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int imageWidth, imageHeight;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

int main(const int argc, char** argv)
{
	// Hide console window
	const HWND h_wnd = GetConsoleWindow();
	ShowWindow(h_wnd, SW_SHOW);
	
	init_glut_glew(argc, argv);
	init_scene();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// SKYBOX
	sky_box_shader = Shader("skybox.vs", "skybox.frag");
	
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
	
	cube_map_texture = LoadCubemap(faces);

	// Main loop
	glutMainLoop();

	return 0;
}
