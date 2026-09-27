#include "pch.h"
//#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "main.h"

#include "Tests.h"
#include "Tests/DemoLight.hpp"

#ifdef _DEBUG
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	Console::InitConsol();

	TestAnomalieTableau::Run();

	Console::DeleteConsol();
	return 0;
}
#else
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	TestCollision::Run();
	
	return 0;
}

#endif // !_DEBUG