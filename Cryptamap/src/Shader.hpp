#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	GLuint ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();
	void use();
};

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	const char* vertexCode;
	const char* fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.open(vertexPath);
	if(!vShaderFile.is_open())
	{
		std::cerr << "ERROR: VERTEX SHADER FILE NOT OPENED AT " << vertexPath << std::endl;
		exit(1);
	}
	fShaderFile.open(fragmentPath);
	if(!fShaderFile.is_open())
	{
		std::cerr << "ERROR: FRAGMENT SHADER FILE NOT OPENED AT " << fragmentPath << std::endl;
		exit(1);
	}

	std::stringstream vertexStream;
	vertexStream << vShaderFile.rdbuf();
	vertexCode = vertexStream.str().c_str();
	vShaderFile.close();

	std::stringstream fragmentStream;
	fragmentStream << fShaderFile.rdbuf();
	fragmentCode = fragmentStream.str().c_str();
	fShaderFile.close();


	int success;
	char infoLog[1024];

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, sizeof(infoLog), NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, sizeof(infoLog), NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	GLuint shader = glCreateProgram();
	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if(success)
	{
		glGetProgramInfoLog(shader, sizeof(infoLog), NULL, infoLog);
		std::cout << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	ID = shader;
}

Shader::~Shader()
{
	glDeleteProgram(ID);
}

void Shader::use()
{
	glUseProgram(ID);
}