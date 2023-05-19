#include "Application.hpp"

void Application::OnEvent(SDL_Event* Event)
{
	switch (Event->type)
	{
		case SDL_QUIT:
		{
			m_Running = false;
		}
	}
}