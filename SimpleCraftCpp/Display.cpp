
#include <GL/glew.h>
#include<iostream>

#include "Display.h"


Display::Display(int width, int height, const std::string& title)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	mGLContext = SDL_GL_CreateContext(mWindow);

	GLenum status = glewInit();
	if (status != GLEW_OK) {

		std::cout << "Glew failed to initalize" << std::endl;
	}

	mClosed = false;
}

Display::~Display()
{
	SDL_GL_DeleteContext(mGLContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Display::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Display::isClosed() {
	return mClosed;
}

void Display::update() {

	SDL_GL_SwapWindow(mWindow);
}

SDL_Window* Display::getWindow()
{
	return mWindow;
}

void Display::setClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}