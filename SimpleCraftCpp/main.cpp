#include <iostream>
#include <fstream>
#include <chrono>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"stb_image.h"

#include "Display.h"
#include "Shader.h"
#include "Camera.h"
#include "Map.h"
#include "HeightGenerator.h"

int width = 800, height = 800;

int mouseX = 0;
int mouseY = 0;

double updateRate = 1.0 / 60.0;

int main(int argc, char** argv) {
	std::cout << "Initializing!" << std::endl;

	//std::cout << HeightGenerator::generateNoise(5, 5) << std::endl;
	//std::cout << HeightGenerator::generateNoise(5, 5) << std::endl;
	//std::cout << HeightGenerator::generateNoise(5, 6) << std::endl;

	Display display(width, height, "SimpleCraftCpp");

	bool KEYS[4096];
	bool LASTKEYS[4096];

	bool MOUSEBUTTON[10];
	bool LASTMOUSEBUTTON[10];

	memset(KEYS, 0, 4096 * sizeof(bool));
	memset(LASTKEYS, 0, 4096 * sizeof(bool));

	memset(MOUSEBUTTON, 0, 10 * sizeof(bool));
	memset(LASTMOUSEBUTTON, 0, 10 * sizeof(bool));
	
	std::chrono::high_resolution_clock::time_point startTime;
	std::chrono::duration<double> deltaTime;
	std::chrono::high_resolution_clock::time_point endTime;

	startTime = std::chrono::high_resolution_clock::now();

	endTime = startTime;
	double unprocessedTime = 0.0f;

	glm::vec3 lightPos(0.0f, 5.0f, 0.0f);

	glm::mat4 lightModel;
	lightModel = glm::translate(lightModel, lightPos);

	glm::mat4 lightProjection = glm::perspective(90.0f, (float)width / (float)height, 0.1f, 1000.0f);

	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans;
	
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	GLfloat vertices[] = {
		// Positions           // Normals           // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	GLfloat skyvertices[] = {
		1.f,  1.f, 0.0f,  // Top Right
		1.f, -1.f, 0.0f,  // Bottom Right
		-1.f, -1.f, 0.0f,  // Bottom Left
		-1.f,  1.f, 0.0f   // Top Left 
	};

	GLuint skyindices[] = {  // Note that we start from 0!
		0, 1, 3,   // First Triangle
		1, 2, 3    // Second Triangle
	};

	char *textureName = "./res/GrassColourSmudge.png";
	int x, y, bytesPerPixel;
	unsigned char* textureData = stbi_load(textureName, &x, &y, &bytesPerPixel, 3);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	//glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	Camera camera = Camera();

	Shader shader = Shader("shader");
	shader.use();

	GLint objectColorLoc = glGetUniformLocation(shader.mProgram, "objectColor");
	glUniform3f(objectColorLoc, 1.0f, 1.f, 1.0f);
	GLint lightColorLoc = glGetUniformLocation(shader.mProgram, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

	GLuint viewPosLoc = glGetUniformLocation(shader.mProgram, "viewPos");
	glUniform3f(viewPosLoc, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

	GLint matSpecularLoc = glGetUniformLocation(shader.mProgram, "material.specular");
	glUniform3f(matSpecularLoc, 1.f, 1.f, 1.f);
	GLint matShineLoc = glGetUniformLocation(shader.mProgram, "material.shininess");
	glUniform1f(matShineLoc, 32.0f);

	glUniform1i(glGetUniformLocation(shader.mProgram, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(shader.mProgram, "material.specular"), 0.0f);

	GLint lightAmbientLoc = glGetUniformLocation(shader.mProgram, "light.ambient");
	glUniform3f(lightAmbientLoc, 0.4f, 0.4f, 0.4f);
	GLint lightDiffuseLoc = glGetUniformLocation(shader.mProgram, "light.diffuse");
	glUniform3f(lightDiffuseLoc, 0.8f, 0.8f, 0.8f);
	GLint lightSpecularLoc = glGetUniformLocation(shader.mProgram, "light.specular");
	glUniform3f(lightSpecularLoc, 0.0f, 0.0f, 0.0f);
	GLint lightPosLoc = glGetUniformLocation(shader.mProgram, "light.position");
	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
	GLint lightDirLoc = glGetUniformLocation(shader.mProgram, "light.direction");
	glm::vec3 lightDir(-0.5f, 0.6f, 0.7f);
	lightDir = glm::normalize(lightDir);
	glUniform3f(lightDirLoc, lightDir.x, lightDir.y, lightDir.z);


	GLint lightConstPos = glGetUniformLocation(shader.mProgram, "light.constant");
	glUniform1f(lightConstPos, 1.0f);

	GLint lightLinearPos = glGetUniformLocation(shader.mProgram, "light.linear");
	glUniform1f(lightLinearPos, 0.09f);

	GLint lightQuadraticPos = glGetUniformLocation(shader.mProgram, "light.quadratic");
	glUniform1f(lightQuadraticPos, 0.032f);

	GLint modelLocation = glGetUniformLocation(shader.mProgram, "model");
	GLint viewLocation = glGetUniformLocation(shader.mProgram, "view");
	GLint projectionLocation = glGetUniformLocation(shader.mProgram, "projection");

	Shader shader2 = Shader("lampshader");
	shader2.use();

	GLint lightModelLocation = glGetUniformLocation(shader2.mProgram, "model");
	GLint lightViewLocation = glGetUniformLocation(shader2.mProgram, "view");
	GLint lightProjectionLocation = glGetUniformLocation(shader2.mProgram, "projection");

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	GLuint lightVBO;
	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	//Copy Vertex array to the GPU
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	Map map;

	display.setClearColor(0.05f, 0.4f, 0.9f, 1.0f);

	while (!display.isClosed()) {

		startTime = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::duration<double>>(startTime - endTime);
		endTime = startTime;
		unprocessedTime += deltaTime.count();

		display.update();
		SDL_Event e;
		memcpy(LASTKEYS, KEYS, sizeof(KEYS));
		memcpy(LASTMOUSEBUTTON, MOUSEBUTTON, sizeof(MOUSEBUTTON));
		while (SDL_PollEvent(&e)) {

			switch (e.type) {
			case SDL_QUIT:
				return 0;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym < 4096 && e.key.keysym.sym > -1)
					KEYS[e.key.keysym.sym] = true;
				break;
			case SDL_KEYUP:
				if (e.key.keysym.sym < 4096 && e.key.keysym.sym > -1)
					KEYS[e.key.keysym.sym] = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (e.button.button < 10 && e.button.button > -1)
					MOUSEBUTTON[e.button.button] = true;
				break;
			case SDL_MOUSEBUTTONUP:
				if (e.button.button < 10 && e.button.button > -1)
					LASTMOUSEBUTTON[e.button.button] = false;
				break;
			case SDL_MOUSEMOTION:
				float xoffset = e.motion.x - width / 2;
				float yoffset = height / 2 - mouseY;

				mouseX = e.motion.x;
				mouseY = e.motion.y;

				SDL_WarpMouseInWindow(display.mWindow, (int)width / 2, (int)height / 2);

				float sensitivity = 0.1;
				xoffset = (float)xoffset * sensitivity;
				yoffset = (float)yoffset * sensitivity;

				camera.rotate(xoffset, yoffset);
				break;
			}
		}

		if (KEYS[SDLK_w])
			camera.forward(deltaTime.count());
		if (KEYS[SDLK_s])
			camera.backward(deltaTime.count());
		if (KEYS[SDLK_a])
			camera.left(deltaTime.count());
		if (KEYS[SDLK_d])
			camera.right(deltaTime.count());

		if (KEYS[SDLK_ESCAPE])
			return 0;

		
		if (MOUSEBUTTON[SDL_BUTTON_LEFT] && !LASTMOUSEBUTTON[SDL_BUTTON_LEFT])
		{
			map.rayCastBlock(camera.getPosition(), camera.getFront());
		}
		
		if (MOUSEBUTTON[SDL_BUTTON_RIGHT] && !LASTMOUSEBUTTON[SDL_BUTTON_RIGHT])
		{
			map.rayCastBlockRemove(camera.getPosition(), camera.getFront());
		}
		

		display.clear();

		
		glm::mat4 lightView;
		lightView = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
		//assert(0);
		//Render Map
		shader.use();

		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(lightView));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(lightProjection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glm::mat4 mapModelMatrix = glm::mat4();
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mapModelMatrix));
		glBindVertexArray(0);

		map.update(camera.getPosition().x, camera.getPosition().z);
		map.render(camera.getPosition().x, camera.getPosition().z);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Render Light
		shader2.use();
		glUniformMatrix4fv(lightModelLocation, 1, GL_FALSE, glm::value_ptr(lightModel));
		glUniformMatrix4fv(lightViewLocation, 1, GL_FALSE, glm::value_ptr(lightView));
		glUniformMatrix4fv(lightProjectionLocation, 1, GL_FALSE, glm::value_ptr(lightProjection));
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		
	}
	//map.stopThreads();
	
	return 0;
}