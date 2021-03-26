#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glsl.h"
#include "objloader.h"
#include "texture.h"
#include "Mesh.h"

using namespace std;

class Object
{
public:
	vector<glm::vec3> normals;
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;

	glm::mat4 model;
	glm::mat4 mv;

	GLuint vao;
	
	GLuint position_id;
	GLuint normal_id;
	GLuint uv_id;
	GLuint texture_id;

	GLuint vbo_vertices;
	GLuint vbo_normals;
	GLuint vbo_uvs;

	Mesh mesh;

	const char* modelPath;
	const char* texturePath;

	Object();
	Object(const char* modelPath, const char* texturePath);
	Object(const char* modelPath, const char* texturePath, Mesh mesh);

	void initModel();
	void initTexture();
	void initBuffers(GLuint program_id);
	void initMatrices(glm::mat4 view);
	void render(glm::mat4 view, GLuint uniform_mv, GLuint uniform_material_ambient, GLuint uniform_material_diffuse, GLuint uniform_specular, GLuint uniform_material_power);
};

