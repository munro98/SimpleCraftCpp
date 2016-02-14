#pragma once
#include <iostream>

#include <GL/glew.h>

#ifndef SHADER_H
#define SHADER_H

class Shader
{
public:
	Shader(std::string name);
	~Shader();

	std::string loadShader(const std::string& fileName);

	void use();

	GLuint mProgram;
private:
	std::string mName;


};
#endif