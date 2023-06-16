#pragma once

#include <SDL2/SDL_rect.h>

struct QuadColliderComponent
{
public:

	SDL_FRect	m_Quad;


public:

	QuadColliderComponent(float x, float y, float w, float h)
		: m_Quad(SDL_FRect(x, y, w, h)) { }

	~QuadColliderComponent() = default;


public:

	operator const SDL_FRect* () { return &m_Quad; }
};
