#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
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
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		const auto message = static_cast<char*>(_malloca(length * sizeof(char)));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static GLuint create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
	const GLuint program = glCreateProgram();
	const GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
	const GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

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

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, nullptr);

	const shader_program_source source = parse_shader("res/shaders/Basic.shader");
	const GLuint shader = create_shader(source.vertex_source, source.fragment_source);
	glUseProgram(shader);

	glutMainLoop();

	glDeleteProgram(shader);

	return 0;
}