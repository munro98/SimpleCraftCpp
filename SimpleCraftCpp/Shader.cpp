#include <fstream>
#include <string>

#include "Shader.h"

Shader::Shader(std::string name) : mName(name)
{
	mProgram = glCreateProgram();

	std::string output = loadShader("./res/" + name + ".vert.c");
	const char *vertexShaderSource = output.c_str();

	std::string output2 = loadShader("./res/" + name + ".frag.c");
	const char *fragmentShaderSource = output2.c_str();

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glAttachShader(mProgram, vertexShader);
	glAttachShader(mProgram, fragmentShader);
	glLinkProgram(mProgram);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error Shader Vertex Compile Fail\n" << "Source File: " << name << std::endl << infoLog << std::endl;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error Shader Fragment Compile Fail\n" << "Source File: " << name << std::endl << infoLog << std::endl;
	}

	glGetShaderiv(mProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error Shader Program Link Fail\n" << "Source File: " << name << std::endl << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


Shader::~Shader()
{
	glDeleteShader(mProgram);
}

std::string Shader::loadShader(const std::string& fileName) {
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;
	
	if (file.is_open()) {
		while (file.good()) {
			getline(file, line);
			
			output.append(line + "\n");
		}

	}
	else {
		std::cerr << "Shader File Load Fail " << fileName << std::endl;
	}

	file.close();

	return output;
}

void Shader::use() {
	glUseProgram(mProgram);
}

void Shader::stop() {
	glUseProgram(0);
}