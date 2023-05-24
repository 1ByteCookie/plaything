#include "Application.hpp"

void Application::OnRender()
{
	SDL_SetRenderDrawColor(m_Renderer, 155, 0, 255, 255);
	SDL_RenderClear(m_Renderer);


	SDL_SetRenderDrawColor(m_Renderer, 0, 255, 255, 255);
	SDL_RenderFillRectF(m_Renderer, &m_Quad);
	SDL_RenderDrawRectF(m_Renderer, &m_Quad);


	SDL_RenderPresent(m_Renderer);
}