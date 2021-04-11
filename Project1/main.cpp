#include <iostream>

#include <GL/glew.h>

#include "Scene.h"

using namespace std;


//--------------------------------------------------------------------------------
// Consts
//--------------------------------------------------------------------------------

const int WIDTH = 1000, HEIGHT = 625;

const char* fragshader_name = "fragmentshader.frag";
const char* vertexshader_name = "vertexshader.vert";

unsigned const int DELTA_TIME = 10;


//--------------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------------

// Scene
Scene scene;

bool isFullscreen = false;

float lastX = 1200;
float lastY = 300;
float pitch = 0.0f;
float yaw = 0.0f;

float deltaForward = 0.0f;
float deltaSide = 0.0f;
float deltaVertical = 0.0f;

float moveUp = 0.0f;
float moveSide = 0.0f;

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void keyboardHandler(unsigned char key, int a, int b)
{
	switch (key) {
	case 27:
		glutExit();
		break;
	case 'w':
		deltaForward = 0.1f;
		break;
	case 's':
		deltaForward = -0.1f;
		break;
	case 'a':
		deltaSide = 0.1f;
		break;
	case 'd':
		deltaSide = -0.1f;
		break;
	case 'i':
		moveUp = 0.01f;
		break;
	case 'k':
		moveUp = -0.01f;
		break;
	case 'j':
		moveSide = 0.01f;
		break;
	case 'l':
		moveSide = -0.01f;
		break;
	case 'v':
		if (scene.cameraMode == 0)
			scene.cameraMode = 1;
		else
			scene.cameraMode = 0;
		break;
	case 'q':
		deltaVertical = -0.1f;
		break;
	case 'e':
		deltaVertical = 0.1f;
		break;
	case 'f':
		if (isFullscreen) {
			glutReshapeWindow(WIDTH, HEIGHT);
		}
		else {
			glutFullScreen();
			glutReshapeWindow(1920, 1080);
		}
		isFullscreen = !isFullscreen;
		break;
	}
}

void keyboardUpHandler(unsigned char key, int a, int b) {
	switch (key) {
	case 'w':
	case 's':
		deltaForward = 0.0f;
		break;
	case 'a':
	case 'd':
		deltaSide = 0.0f;
		break;
	case 'i':
	case 'k':
		moveUp = 0.0f;
		break;
	case 'j':
	case 'l':
		moveSide = 0.0f;
		break;
	case 'q':
	case 'e':
		deltaVertical = 0.0f;
		break;
	}
}

//--------------------------------------------------------------------------------
// Keyboard handling
//--------------------------------------------------------------------------------

void mouseHandler(int x, int y)
{
	float xOffset = x - lastX;
	float yOffset = y - lastY;
	lastX = (float)x;
	lastY = (float)y;

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
	Camera cam;
	if (scene.cameraMode == 0)
		cam = scene.camera;
	else
		cam = scene.drone;
	cam.m_CameraFront = glm::normalize(direction);
	if (scene.cameraMode == 0)
		scene.camera = cam;
	else
		scene.drone = cam;
}

//--------------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------------

void Render()
{
	Camera cam;
	if (scene.cameraMode == 0)
		cam = scene.camera;
	else
		cam = scene.drone;

	if (deltaForward) {
		cam.m_CameraPos += deltaForward * cam.m_CameraFront;
	}
	if (deltaSide) {
		cam.m_CameraPos -= glm::normalize(glm::cross(cam.m_CameraFront, cam.m_CameraUp)) * deltaSide;
	}
	if (deltaVertical && scene.cameraMode == 1) {
		cam.m_CameraPos.y += deltaVertical;
	}
	if (moveUp) {
		cam.m_CameraFront.y += moveUp;
	}
	if (moveSide) {
		cam.m_CameraFront.x -= moveSide;
	}
	if (scene.cameraMode == 0) {
		cam.m_CameraPos.y = 1.75f;
	}
	
	if (scene.cameraMode == 0)
		scene.camera = cam;
	else
		scene.drone = cam;
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
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Hello OpenGL");
	glutDisplayFunc(Render);
	glutKeyboardFunc(keyboardHandler);
	glutIgnoreKeyRepeat(1);
	glutKeyboardUpFunc(keyboardUpHandler);
	glutPassiveMotionFunc(mouseHandler);
	glutTimerFunc(DELTA_TIME, Render, 0);
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
	ShowWindow(hWnd, SW_HIDE);

	// Main loop
	glutMainLoop();

	return 0;
}
