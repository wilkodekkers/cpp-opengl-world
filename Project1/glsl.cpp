#include "glsl.h"

char* glsl::contents_;

glsl::glsl()
{
	
}

glsl::~glsl()
{
	
}


char* glsl::read_file(const char* filename)
{
    // Open the file
    FILE* fp = fopen(filename, "r");
    // Move the file pointer to the end of the file and determing the length
    fseek(fp, 0, SEEK_END);
    const long file_length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    const auto contents = new char[file_length + 1];
    // zero out memory
    for (int i = 0; i < file_length + 1; i++) 
    {
        contents[i] = 0;
    }
    // Here's the actual read
    fread(contents, 1, file_length, fp);
    // This is how you denote the end of a string in C
    contents[file_length + 1] = '\0';
    fclose(fp);
    return contents;
}

bool glsl::compiled_status(GLint shader_id)
{
    GLint compiled = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
    if (compiled) {
        return true;
    }
    else {
        GLint log_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
        const auto msg_buffer = new char[log_length];
        glGetShaderInfoLog(shader_id, log_length, nullptr, msg_buffer);
        printf("%s\n", msg_buffer);
        delete[] (msg_buffer);
        return false;
    }
}

GLuint glsl::make_vertex_shader(const char* shader_source)
{
	const GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &shader_source, nullptr);
    glCompileShader(vertex_shader_id);
	const bool compiled_correctly = compiled_status(vertex_shader_id);
    if (compiled_correctly)
    {
        return vertex_shader_id;
    }
    return -1;
}

GLuint glsl::make_fragment_shader(const char* shader_source)
{
	const GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &shader_source, nullptr);
    glCompileShader(fragment_shader_id);
    //delete[] source;
	const bool compiled_correctly = compiled_status(fragment_shader_id);
    if (compiled_correctly) {
        return fragment_shader_id;
    }
    return -1;
}

GLuint glsl::make_shader_program(const GLuint vertex_shader_id, const GLuint fragment_shader_id)
{
	const GLuint shader_id = glCreateProgram();
    glAttachShader(shader_id, vertex_shader_id);
    glAttachShader(shader_id, fragment_shader_id);
    glLinkProgram(shader_id);
    return shader_id;
}