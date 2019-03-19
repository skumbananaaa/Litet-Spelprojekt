#include <System/Application.h>
#include "../Include/Game.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Application* pApp = new Game();
	int res = pApp->Run();
	delete pApp;

	return res;
}