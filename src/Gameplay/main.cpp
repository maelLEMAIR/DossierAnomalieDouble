#include "pch.h"
#include <windows.h>
#include "main.h"

#include "GameManager.h"
#include "SplashScreenScene.h"

#ifdef _DEBUG
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    Console::InitConsol();
    
    /////////////////////////////////////////////////////////////////////////////
    GameManager gameManager;

    gameManager.Run();
    
    Console::DeleteConsol();
    return 0;
}
#else
int WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    GameManager gameManager;

    gameManager.Run();
    
    return 0;
}

#endif // !_DEBUG