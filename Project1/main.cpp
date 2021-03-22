#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Material.h"
#include "Object.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const int WIDTH = 800, HEIGHT = 600;

const char* fragshader_name = "fragmentshader.frag";
const char* vertexshader_name = "vertexshader.vert";

unsigned const int DELTA_TIME = 10;


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

// ID's
GLuint program_id;

// Uniform ID's
GLuint uniform_mv;
GLuint uniform_material_ambient;
GLuint uniform_material_diffuse;
GLuint uniform_specular;
GLuint uniform_material_power;

// Matrices
glm::mat4 view;
glm::mat4 projection;

// Material and light
glm::vec3 light_position;

// Mesh variables
Object objects[3];

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b)
{
	if (key == 27)
		glutExit();
}


//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Attach to program_id
	glUseProgram(program_id);

	for (int i = 0; i < 3; i++) {
		objects[i].render(view, uniform_mv, uniform_material_ambient, uniform_material_diffuse, uniform_specular, uniform_material_power);
	}

	// Swap buffers
	glutSwapBuffers();
}


//------------------------------------------------------------
// void Render(int n)
// Render method that is called by the timer function
//------------------------------------------------------------

void Render(int n)
{
	Render();
	glutTimerFunc(DELTA_TIME, Render, 0);
}


//------------------------------------------------------------
// void InitGlutGlew(int argc, char **argv)
// Initializes Glut and Glew
//------------------------------------------------------------

void InitGlutGlew(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Hello OpenGL");
	glutDisplayFunc(Render);
	glutKeyboardFunc(keyboardHandler);
	glutTimerFunc(DELTA_TIME, Render, 0);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	glewInit();
}


//------------------------------------------------------------
// void InitShaders()
// Initializes the fragmentshader and vertexshader
//------------------------------------------------------------

void InitShaders()
{
	char* vertexshader = glsl::readFile(vertexshader_name);
	GLuint vsh_id = glsl::makeVertexShader(vertexshader);

	char* fragshader = glsl::readFile(fragshader_name);
	GLuint fsh_id = glsl::makeFragmentShader(fragshader);

	program_id = glsl::makeShaderProgram(vsh_id, fsh_id);
}


//------------------------------------------------------------
// void InitMatrices()
//------------------------------------------------------------

void InitMatrices()
{
	view = glm::lookAt(
		glm::vec3(2.0, 2.0, 10.0),  // eye
		glm::vec3(4.0, 0.0, 0.0),  // center
		glm::vec3(0.0, 1.0, 0.0));  // up
	projection = glm::perspective(
		glm::radians(45.4f),
		1.0f * WIDTH / HEIGHT,
		0.1f,
		20.0f);
	light_position = glm::vec3(4.0f, 4.0f, 4.0f);
	for (int i = 0; i < 3; i++) {
		objects[i].initMatrices(view);
	}
}


//------------------------------------------------------------
// void InitBuffers()
// Allocates and fills buffers
//------------------------------------------------------------

void InitBuffers()
{
	for (int i = 0; i < 3; i++) {
		objects[i].initBuffers(program_id);
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
	glUseProgram(program_id);

	// Fill uniform vars
	glUniformMatrix4fv(uniform_proj, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3fv(uniform_light_pos, 1, glm::value_ptr(light_position));
}

void InitObjects()
{
	for (int i = 0; i < 3; i++) {
		Object object = Object("Objects/teapot.obj", "Textures/Yellobrk.bmp");
		object.initModel();
		object.initTexture();
		objects[i] = object;
	}
}

int main(int argc, char** argv)
{
	InitGlutGlew(argc, argv);
	InitObjects();
	InitShaders();
	InitMatrices();
	InitBuffers();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Hide console window
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);

	// Main loop
	glutMainLoop();

	return 0;
}
