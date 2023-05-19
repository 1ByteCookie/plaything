#pragma once

#ifdef _DEBUG
	#define DebugLog() Utils::PrintErrorSDL();

#else
#define DebugLog()

#endif


namespace Utils
{
	void PrintErrorSDL();
}
