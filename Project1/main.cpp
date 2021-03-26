#include <iostream>

#include <GL/glew.h>

#include "Scene.h"

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

// Scene
Scene scene;

float lastX = 400;
float lastY = 300;
float pitch = 0.0f;
float yaw = 0.0f;

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b)
{
	const float cameraSpeed = 0.1f;
	const float rotateSpeed = 0.2f;
	Camera camera = scene.camera;
	if (key == 27)
		glutExit();
	if (key == 'w')
		camera.m_CameraPos += cameraSpeed * camera.m_CameraFront;
	if (key == 's')
		camera.m_CameraPos -= cameraSpeed * camera.m_CameraFront;
	if (key == 'a')
		camera.m_CameraPos -= glm::normalize(glm::cross(camera.m_CameraFront, camera.m_CameraUp)) * cameraSpeed;
	if (key == 'd')
		camera.m_CameraPos += glm::normalize(glm::cross(camera.m_CameraFront, camera.m_CameraUp)) * cameraSpeed;
	camera.m_CameraPos.y = 1.75f;
	scene.camera = camera;
}

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void mouseHandler(int x, int y) 
{
	float xOffset = x - lastX;
	float yOffset = y - lastY;
	lastX = x;
	lastY = y;

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch -= yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	scene.camera.m_CameraFront = glm::normalize(direction);
}

//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
	scene.render();
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
	glutInitWindowSize(GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT);
	glutCreateWindow("Hello OpenGL");
	glutDisplayFunc(Render);
	glutKeyboardFunc(keyboardHandler);
	glutPassiveMotionFunc(mouseHandler);
	glutTimerFunc(DELTA_TIME, Render, 0);
	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	glewInit();
}

void InitScene()
{
	scene = Scene();
	scene.init(fragshader_name, vertexshader_name, GLUT_SCREEN_WIDTH, GLUT_SCREEN_HEIGHT);
}

int main(int argc, char** argv)
{
	InitGlutGlew(argc, argv);
	InitScene();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Hide console window
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_SHOW);

	// Main loop
	glutMainLoop();

	return 0;
}
