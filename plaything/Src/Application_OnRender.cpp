#include "Application.hpp"

#include "Components/QuadComponent.hpp"
#include "Components/Tags.hpp"

void Application::OnRender()
{
	SDL_SetRenderDrawColor(m_Renderer, 155, 0, 255, 255);
	SDL_RenderClear(m_Renderer);


	auto PlayerView = m_Scene.view<QuadComponent, Tags::Player>();
	for(auto Entity : PlayerView)
	{
		auto& Quad = m_Scene.get<QuadComponent>(Entity);

		SDL_SetRenderDrawColor(m_Renderer, 0, 255, 255, 255);
		SDL_RenderFillRectF(m_Renderer, Quad);
		SDL_RenderDrawRectF(m_Renderer, Quad);

	}

	auto EnemyView = m_Scene.view<QuadComponent, Tags::Enemy>();
	for (auto Entity : EnemyView)
	{
		auto& Quad = m_Scene.get<QuadComponent>(Entity);

		SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 255);
		SDL_RenderFillRectF(m_Renderer, Quad);
		SDL_RenderDrawRectF(m_Renderer, Quad);

	}

	SDL_RenderPresent(m_Renderer);
}