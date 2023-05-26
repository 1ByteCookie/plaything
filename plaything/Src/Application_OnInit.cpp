#include "Application.hpp"

#include "Logging.hpp"

#include "Components/QuadComponent.hpp"

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

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!m_Renderer)
	{
		DebugLog();
		return false;
	}


	InitScene();

	return true;
}

void Application::InitScene()
{
	entt::entity Player = m_Scene.create();
	m_Scene.emplace<QuadComponent>(Player, 10, 10, 60, 60);
}