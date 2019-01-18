#include <EnginePch.h>
#include <System/Window.h>

Window::Window(int width, int height)
{
	m_pWindow = glfwCreateWindow(width, height, "Lilla Spel", nullptr, nullptr);
	if (m_pWindow == nullptr)
	{
		std::cout << "Failed to create window" << std::endl;
	}
}


Window::~Window()
{
}


void Window::PollEvents()
{
}