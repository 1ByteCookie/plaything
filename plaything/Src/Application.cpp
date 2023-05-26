#include "Application.hpp"

Application::Application()
	: m_Window(nullptr)
	, m_Renderer(nullptr)
	
	, m_Title("plaything")
	, m_Width(1280)
	, m_Height(720)

	, m_Running(true)
{

}

int Application::OnExecute()
{
	if (!OnInit())
		return -1;
	
	SDL_Event Event{};

	while (m_Running)
	{
		while (SDL_PollEvent(&Event))
		{
			OnEvent(&Event);
		}

		OnLoop();
		OnRender();
	}

	OnCleanup();

	return 0;
}