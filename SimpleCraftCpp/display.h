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
	bool getIsClosed();
	void close();

	SDL_Window* m_window;

private:	
	
	SDL_GLContext m_GLContext;
	bool m_IsClosed;
};

