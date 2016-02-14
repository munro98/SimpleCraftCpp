#pragma once
#include<SDL2/SDL.h>

class Display
{
public:
	Display(int width, int height, const std::string& title);
	~Display();

	void setClearColor(float r, float g, float b, float a);
	void clear();
	void update();

	SDL_Window * getWindow();
	bool isClosed();

	SDL_Window* mWindow;

private:	
	
	SDL_GLContext mGLContext;
	bool mClosed;
};

