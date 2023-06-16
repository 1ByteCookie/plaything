#include "Application.hpp"

#include "Components/QuadComponent.hpp"
#include "Components/QuadColliderComponent.hpp"
#include "Components/SpeedComponent.hpp"
#include "Components/Tags.hpp"

#include "Vector2.hpp"

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
			PlayerMovement(Vector2{ 0, -1 });
			break;
		}

		case SDL_SCANCODE_A:
		{
			PlayerMovement(Vector2{ -1, 0 });
			break;
		}

		case SDL_SCANCODE_S:
		{
			PlayerMovement(Vector2{ 0, 1 });
			break;
		}

		case SDL_SCANCODE_D:
		{
			PlayerMovement(Vector2{ 1, 0 });
			break;
		}
	}
}

void Application::PlayerMovement(Vector2 Direction)
{
	auto Player	= m_Scene.view<QuadComponent, Tags::Player>().front();

	auto& PlayerTransform	= m_Scene.get<QuadComponent>(Player);
	auto& PlayerCollider	= m_Scene.get<QuadColliderComponent>(Player);
	float PlayerSpeed		= m_Scene.get<SpeedComponent>(Player).Speed;


	PlayerTransform.m_Quad.x += Direction.x * PlayerSpeed;
	PlayerTransform.m_Quad.y += Direction.y * PlayerSpeed;

	PlayerCollider.m_Quad.x  += Direction.x * PlayerSpeed;
	PlayerCollider.m_Quad.y  += Direction.y * PlayerSpeed;
}