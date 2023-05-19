#include "Application.hpp"

#include <iostream>

bool Application::OnInit()
{
	m_Window = SDL_CreateWindow(m_Title.c_str(),
								SDL_WINDOWPOS_CENTERED,
								SDL_WINDOWPOS_CENTERED,
								m_Width,
								m_Height,
								SDL_WINDOW_SHOWN);

	if (!m_Window)
	{
		std::string Message{SDL_GetError()};
		std::cout << Message << std::endl;

		return false;
	}

	return true;
}