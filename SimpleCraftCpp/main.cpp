#include <iostream>
#include <fstream>
#include <chrono>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Display.h"
#include "Shader.h"
#include "Camera.h"

#include "Frustum.h"
#include "World.h"

int width = 800, height = 600;
//int width = 2540, height = 1400;

int mouseX = 0;
int mouseY = 0;

double updateRate = 1.0 / 60.0;

int main(int argc, char** argv) {
	std::cout << "Initializing!" << std::endl;
	//std::cout << sizeof(Block) << std::endl;

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

	char *textureName = "./res/TextureMap.png";
	int x, y, bytesPerPixel;
	unsigned char* textureData = stbi_load(textureName, &x, &y, &bytesPerPixel, 3);

	GLuint textureAtlas;
	glGenTextures(1, &textureAtlas);
	glBindTexture(GL_TEXTURE_2D, textureAtlas);

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
	glUniform3f(lightDiffuseLoc, 1.0f, 1.0f, 1.0f);
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

	GLint viewprojectionLocation = glGetUniformLocation(shader.mProgram, "viewprojection");


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	World world;

	display.setClearColor(0.05f, 0.4f, 0.9f, 1.0f);

	glm::mat4 view;
	glm::mat4 projection = glm::perspective(60.0f, (float)width / (float)height, 0.1f, 1000.0f);

	while (!display.getIsClosed()) {

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
				display.close();
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

				SDL_WarpMouseInWindow(display.m_window, (int)width / 2, (int)height / 2);

				float sensitivity = 0.1;
				xoffset = (float)xoffset * sensitivity;
				yoffset = (float)yoffset * sensitivity;

				camera.rotate(xoffset, yoffset);
				break;
			}
		}

		glm::vec3 cameraVelocity(0.0f, 0.0f, 0.0f);
		float speed = 10.0f;

		if (KEYS[SDLK_w])
			cameraVelocity += camera.getFront() * speed * (float)deltaTime.count();
			//camera.forward(deltaTime.count());
		if (KEYS[SDLK_s])
			cameraVelocity -= camera.getFront() * speed * (float)deltaTime.count();
		if (KEYS[SDLK_a])
			cameraVelocity -= speed * (float)deltaTime.count()  * glm::normalize(glm::cross(camera.getFront(), camera.getUp()));
		if (KEYS[SDLK_d])
			cameraVelocity += speed * (float)deltaTime.count() * glm::normalize(glm::cross(camera.getFront(), camera.getUp()));

		// Collision
		/*
		if (world.hitBlock(camera.getPosition() + glm::vec3(cameraVelocity.x, 0.0f, 0.0f)))
		{
			cameraVelocity.x = 0.0f;
		}

		if (world.hitBlock(camera.getPosition() + glm::vec3(0.0f, cameraVelocity.y, 0.0f)))
		{
			cameraVelocity.y = 0.0f;
		}

		if (world.hitBlock(camera.getPosition() + glm::vec3(0.0f, 0.0f, cameraVelocity.z)))
		{
			cameraVelocity.z = 0.0f;
		}
		*/
		camera.setPosition(camera.getPosition() + cameraVelocity);

		if (KEYS[SDLK_ESCAPE])
			display.close();

		
		if (MOUSEBUTTON[SDL_BUTTON_LEFT] && !LASTMOUSEBUTTON[SDL_BUTTON_LEFT])
		{
			world.rayCastBlockRemove(camera.getPosition(), camera.getFront());
		}
		
		if (MOUSEBUTTON[SDL_BUTTON_RIGHT] && !LASTMOUSEBUTTON[SDL_BUTTON_RIGHT])
		{
			world.rayCastBlock(camera.getPosition(), camera.getFront());
		}
		

		display.clear();


		//Make viewProjection Matrix and frustum
		view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getFront(), camera.getUp());
		Frustum frustum(view, projection);
		glm::mat4 viewProjection = projection * view;

		//Render Map
		shader.use();

		glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(viewPosLoc, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
		
		//glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(viewprojectionLocation, 1, GL_FALSE, glm::value_ptr(viewProjection));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureAtlas);

		glm::mat4 mapModelMatrix = glm::mat4();
		//glm::translate(mapModelMatrix, camera.getPosition());
		//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(mapModelMatrix));
		glBindVertexArray(0);

		world.update(camera.getPosition().x, camera.getPosition().z);
		world.render(frustum, camera.getPosition().x, camera.getPosition().z);
		glBindTexture(GL_TEXTURE_2D, 0);

		shader.stop();

	}
	world.stopThreads();
	
	return 0;
}