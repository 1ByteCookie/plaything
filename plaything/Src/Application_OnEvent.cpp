#include "Application.hpp"

#include <iostream>

void Application::OnEvent(SDL_Event* Event)
{
	switch (Event->type)
	{
		case SDL_QUIT:
		{
			m_Running = false;
		}
	}

	HandlePlayerInput(Event);
}

void Application::HandlePlayerInput(SDL_Event* Event)
{
	switch (Event->key.keysym.scancode)
	{
		case SDL_SCANCODE_W:
		{
			std::cout << "W key has been pressed!" << std::endl;
			break;
		}

		case SDL_SCANCODE_A:
		{
			std::cout << "A key has been pressed!" << std::endl;
			break;
		}

		case SDL_SCANCODE_S:
		{
			std::cout << "S key has been pressed!" << std::endl;
			break;
		}

		case SDL_SCANCODE_D:
		{
			std::cout << "D key has been pressed!" << std::endl;
			break;
		}
	}
}