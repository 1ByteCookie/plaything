#include "Application.hpp"

#include "Logging.hpp"

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
		DebugLog();

		return false;
	}

	return true;
}