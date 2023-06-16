#include "Application.hpp"

#include "Components/QuadColliderComponent.hpp"
#include "Components/Tags.hpp"

void Application::OnLoop()
{
	auto PlayerView	= m_Scene.view<QuadColliderComponent, Tags::Player>();
	auto EnemyView	= m_Scene.view<QuadColliderComponent, Tags::Enemy>();

	auto& PlayerCollider = m_Scene.get<QuadColliderComponent>(PlayerView.front());
	
	for (auto Entity : EnemyView)
	{
		auto& EnemyCollider = m_Scene.get<QuadColliderComponent>(Entity);

		if (SDL_HasIntersectionF(PlayerCollider, EnemyCollider) == SDL_TRUE)
		{
			m_Scene.destroy(Entity);
		}
	}
}