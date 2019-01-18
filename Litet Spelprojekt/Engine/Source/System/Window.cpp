#include <EnginePch.h>
#include "..\..\Include\System\Window.h"

Window::Window(int width, int height)
{
	//Assumes only one window in app
	if (!glfwInit())
		std::cout << "Failed to init GLFW" << std::endl;
}


Window::~Window()
{
	glfwTerminate();
}


void Window::PollEvents()
{
	
}