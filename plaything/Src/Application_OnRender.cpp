#include "Application.hpp"

#include "Components/QuadComponent.hpp"

void Application::OnRender()
{
	SDL_SetRenderDrawColor(m_Renderer, 155, 0, 255, 255);
	SDL_RenderClear(m_Renderer);


	auto View = m_Scene.view<QuadComponent>();
	for(auto Entity : View)
	{
		auto& Quad = m_Scene.get<QuadComponent>(Entity);

		SDL_SetRenderDrawColor(m_Renderer, 0, 255, 255, 255);
		SDL_RenderFillRectF(m_Renderer, Quad);
		SDL_RenderDrawRectF(m_Renderer, Quad);

	}

	SDL_RenderPresent(m_Renderer);
}