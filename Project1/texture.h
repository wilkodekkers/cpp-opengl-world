#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <SOIL2.h>

// Load a .BMP file using our custom loader
GLuint loadBMP(const char * imagepath);

//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

static GLuint load_cube_map(std::vector<const GLchar* > faces)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);

	int image_width, image_height;

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

	for (GLuint i = 0; i < faces.size(); i++)
	{
		unsigned char* image = SOIL_load_image(faces[i], &image_width, &image_height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return texture_id;
}

#endif