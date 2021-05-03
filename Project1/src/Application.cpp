#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define glCall(x) glClearError();\
	x;\
	ASSERT(glLogCall(#x, __FILE__, __LINE__));

const int width = 1280;
const int height = 720;

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

struct shader_program_source {
	std::string vertex_source;
	std::string fragment_source;
};

static void glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool glLogCall(const char* function, const char* file, int line)
{
	while (const GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << std::hex << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	
	return true;
}

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
	glCall(glShaderSource(id, 1, &src, nullptr));
	glCall(glCompileShader(id));

	int result;
	glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		const auto message = static_cast<char*>(_malloca(length * sizeof(char)));
		glCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

static GLuint create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	const GLuint program = glCreateProgram();
	const GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	const GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	glCall(glAttachShader(program, vs));
	glCall(glAttachShader(program, fs));
	glCall(glLinkProgram(program));
	glCall(glValidateProgram(program));

	glCall(glDeleteShader(vs));
	glCall(glDeleteShader(fs));

	return program;
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	
	glutSwapBuffers();
}

void init_glut(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2);
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
	
	GLuint buffer;
	glCall(glGenBuffers(1, &buffer));
	glCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	glCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

	glCall(glEnableVertexAttribArray(0));
	glCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr));

	GLuint ibo;
	glCall(glGenBuffers(1, &ibo));
	glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	const shader_program_source source = parse_shader("res/shaders/Basic.shader");
	const GLuint shader = create_shader(source.vertex_source, source.fragment_source);
	glCall(glUseProgram(shader));

	glutMainLoop();

	glCall(glDeleteProgram(shader));

	return 0;
}