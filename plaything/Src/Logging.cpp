#include "Logging.hpp"

#include <SDL2/SDL.h>
#include <iostream>

namespace Utils
{
	void PrintErrorSDL()
	{
		std::cout << SDL_GetError() << std::endl;
	}
}