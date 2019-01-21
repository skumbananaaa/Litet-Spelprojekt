#include "..\Include\Game.h"

int main(int argc, char* argv[])
{
	Application* pApp = new Game();
	int res = pApp->Run();
	delete pApp;

	std::cin.get();
	return res;
}