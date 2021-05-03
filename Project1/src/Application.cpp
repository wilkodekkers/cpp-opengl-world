#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

const int width = 1280;
const int height = 720;

GLuint vao;

GLuint shader;
float r = 0.0f;
float increment = 0.05f;

float positions[] = {
	-0.5f, -0.5f,
	 0.5f, -0.5f,
	 0.5f,  0.5f,
	-0.5f,  0.5f
};

unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};

index_buffer* ib;
vertex_array* va;

struct shader_program_source {
	std::string vertex_source;
	std::string fragment_source;
};

static shader_program_source parse_shader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class shader_type
	{
		none = -1, vertex = 0, fragment = 1
	};

	std::string line;
	std::stringstream ss[2];
	auto type = shader_type::none;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = shader_type::vertex;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = shader_type::fragment;
			}
		}
		else
		{
			ss[static_cast<int>(type)] << line << "\n";
		}
	}

	return { ss[0].str(), ss[1].str() }; 
}

static unsigned int compile_shader(const GLuint type, const std::string& source)
{
	const GLuint id = glCreateShader(type);
	const char* src = source.c_str();
	glCall(glShaderSource(id, 1, &src, nullptr))
	glCall(glCompileShader(id))

	int result;
	glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length))
		const auto message = static_cast<char*>(_malloca(length * sizeof(char)));
		glCall(glGetShaderInfoLog(id, length, &length, message))
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glCall(glDeleteShader(id))
		return 0;
	}

	return id;
}

static GLuint create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	const GLuint program = glCreateProgram();
	const GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	const GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	glCall(glAttachShader(program, vs))
	glCall(glAttachShader(program, fs))
	glCall(glLinkProgram(program))
	glCall(glValidateProgram(program))

	glCall(glDeleteShader(vs))
	glCall(glDeleteShader(fs))

	return program;
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCall(glUseProgram(shader))
	glCall(const int location = glGetUniformLocation(shader, "u_Color"));
	glCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
	
	va->bind();
	ib->bind();
	
	glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0))

	if (r > 1.0f)
	{
		increment = -0.05f;
	}
	else if (r < 0.0f)
	{
		increment = 0.05f;
	}

	r += increment;
	
	glutSwapBuffers();
}

void render(int delta)
{
	render();
	glutTimerFunc(1, render, 0);
}

void init_glut(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2);
	glutCreateWindow("Wilko Dekkers | OpenGL Game");
	glutDisplayFunc(render);
	glutTimerFunc(1, render, 0);

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	glewInit();
}

int main(const int argc, char** argv)
{
	init_glut(argc, argv);

	glCall(glGenVertexArrays(1, &vao));
	glCall(glBindVertexArray(vao));

	vertex_array nva;
	va = &nva;
	vertex_buffer vb(positions, 4 * 2 * sizeof(float));

	vertex_buffer_layout layout;
	layout.push<float>(2);
	va->add_buffer(vb, layout);
	
	auto nib = index_buffer(indices, 6);
	ib = &nib;
	
	const shader_program_source source = parse_shader("res/shaders/Basic.shader");
	shader = create_shader(source.vertex_source, source.fragment_source);
	glCall(glUseProgram(shader))

	glCall(const int location = glGetUniformLocation(shader, "u_Color"))
	glCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f))

	glutMainLoop();

	glCall(glDeleteProgram(shader))

	return 0;
}