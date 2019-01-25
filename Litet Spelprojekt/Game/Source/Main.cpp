#include <System/Application.h>
#include <iostream>
#include <crtdbg.h>

#include "..\Include\Game.h"

int main(int argc, char* argv[])
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Application* pApp = new Game();
	int res = pApp->Run();
	delete pApp;

	return res;
}