#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Shader
{
public:
	GLuint ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void use();
};