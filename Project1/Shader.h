#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class shader
{
    GLuint program_;
public:
	shader()
	{
		
	}
	
    shader(const GLchar* vertex_path, const GLchar* fragment_path)
    {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream v_shader_file;
        std::ifstream f_shader_file;
		
        // ensures ifstream objects can throw exceptions:
        v_shader_file.exceptions(std::ifstream::badbit);
        f_shader_file.exceptions(std::ifstream::badbit);
		
        try
        {
            // Open files
            v_shader_file.open(vertex_path);
            f_shader_file.open(fragment_path);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << v_shader_file.rdbuf();
            fShaderStream << f_shader_file.rdbuf();
            // close file handlers
            v_shader_file.close();
            f_shader_file.close();
            // Convert stream into string
            vertex_code = vShaderStream.str();
            fragment_code = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
		
        const GLchar* v_shader_code = vertex_code.c_str();
        const GLchar* f_shader_code = fragment_code.c_str();

		// Compile shaders
        GLuint vertex, fragment;
        GLint success;
        GLchar info_log[512];

		// Vertex Shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v_shader_code, nullptr);
        glCompileShader(vertex);

		// Print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, nullptr, info_log);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
        }

		// Fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &f_shader_code, nullptr);
        glCompileShader(fragment);
		
        // Print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, nullptr, info_log);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
        }
		
        // Shader Program
        this->program_ = glCreateProgram();
        glAttachShader(this->program_, vertex);
        glAttachShader(this->program_, fragment);
        glLinkProgram(this->program_);

		// Print linking errors if any
        glGetProgramiv(this->program_, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(this->program_, 512, nullptr, info_log);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
        }
        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader(vertex);
        glDeleteShader(fragment);

    }

    GLuint get_program() const { return program_; }
	
    void use() const
    {
        glUseProgram(this->program_);
    }
};

#endif