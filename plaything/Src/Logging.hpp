#pragma once

namespace Utils
{
	void PrintErrorSDL();
}


#ifdef _DEBUG
	#define DebugLog() Utils::PrintErrorSDL();

#else
#define DebugLog()

#endif