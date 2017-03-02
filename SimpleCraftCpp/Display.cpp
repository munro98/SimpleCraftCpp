
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

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	m_GLContext = SDL_GL_CreateContext(m_window);

	GLenum status = glewInit();
	if (status != GLEW_OK) {

		std::cout << "Glew failed to initalize" << std::endl;
	}

	m_IsClosed = false;
}

Display::~Display()
{
	SDL_GL_DeleteContext(m_GLContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Display::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Display::getIsClosed() {
	return m_IsClosed;
}

void Display::close() {
	m_IsClosed = true;
}

void Display::update() {

	SDL_GL_SwapWindow(m_window);
}

SDL_Window* Display::getWindow()
{
	return m_window;
}

void Display::setClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}