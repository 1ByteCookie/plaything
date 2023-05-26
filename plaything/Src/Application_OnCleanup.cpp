#include "Application.hpp"

void Application::OnCleanup()
{
	m_Scene.clear();

	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);
}