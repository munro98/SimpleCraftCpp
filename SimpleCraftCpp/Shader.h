#pragma once
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
	Shader(std::string name);
	~Shader();

	std::string loadShader(const std::string& fileName);

	void use();

	static void stop();

	GLuint mProgram;
private:
	std::string mName;


};
