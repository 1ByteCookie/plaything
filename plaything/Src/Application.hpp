#pragma once

#include "SDL2/SDL.h"

#include <string>

class Application
{

public:

	Application();
	~Application() = default;

	int OnExecute();


public:

	bool OnInit();
	
	void OnEvent(SDL_Event* Event);
	
	void OnLoop();
	
	void OnRender();
	
	void OnCleanup();


private:

	SDL_Window*		m_Window;
	std::string		m_Title;
	
	uint32_t		m_Width;
	uint32_t		m_Height;

	SDL_Renderer*	m_Renderer;

	SDL_FRect		m_Quad;

	bool			m_Running;

};