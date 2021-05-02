#include <GL/glew.h>
#include <GL/glut.h>

const int width = 1280;
const int height = 720;

GLuint buffer;
float positions[6] = {
	-0.5f, -0.5f,
	 0.0f,  0.5f,
	 0.5f, -0.5f
};

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
}

void init_glut(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(width / 2, height / 4);
	glutCreateWindow("Wilko Dekkers | OpenGL Game");
	glutDisplayFunc(render);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	glewInit();
}

int main(const int argc, char** argv)
{
	init_glut(argc, argv);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

	// Hide console window
	const HWND h_wnd = GetConsoleWindow();
	ShowWindow(h_wnd, SW_HIDE);

	glutMainLoop();

	return 0;
}