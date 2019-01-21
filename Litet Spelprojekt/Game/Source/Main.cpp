#include <System/Application.h>
#include <iostream>

int main(int argc, char* argv[])
{
	Application* pApp = new Application();
	int res = pApp->Run();
	delete pApp;

	std::cin.get();
	return res;
}