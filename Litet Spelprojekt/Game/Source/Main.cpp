#include <System/Application.h>
#include <iostream>
#include <crtdbg.h>

#include "..\Include\Game.h"

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	Application* pApp = new Game();
	int res = pApp->Run();
	delete pApp;

	std::cin.get();
	return res;
}