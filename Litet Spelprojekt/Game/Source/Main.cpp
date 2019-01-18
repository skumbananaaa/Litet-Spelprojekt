#include <System/Application.h>
#include <iostream>

int main(int argc, char* argv[])
{
	Application* pApp = new Application();
	//TODO: Call run here
	delete pApp;

	std::cin.get();
	return 0;
}