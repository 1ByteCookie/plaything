#pragma once

#include <SDL2/SDL_rect.h>

// component wrapper of SDL_FRect

struct QuadComponent
{

public:

	SDL_FRect	m_Quad;


public:
	
	QuadComponent(float x, float y, float w, float h)
		: m_Quad(SDL_FRect(x, y, w, h)) { }
	
	~QuadComponent() = default;


public:

	operator const SDL_FRect* () { return &m_Quad; }

};