#pragma once

#include <SDL2/SDL.h>
#include <entt/entt.hpp>

#include <string>

constexpr uint32_t TileW = 100;
constexpr uint32_t TileH = 100;

class Application
{

public:

	Application();
	~Application() = default;

	int OnExecute();


private:

	bool OnInit();
	void OnEvent(SDL_Event* Event);
	void OnLoop();
	void OnRender();
	void OnCleanup();


private:

	void InitResource();
	void HandlePlayerInput(SDL_Event* Event);
	void PlayerMovement(class Vector2 Direction);


private:

	SDL_Window*		m_Window;
	SDL_Renderer*	m_Renderer;
	entt::registry	m_Scene;

	std::string		m_Title;
	uint32_t		m_Width;
	uint32_t		m_Height;

	bool			m_Running;

};