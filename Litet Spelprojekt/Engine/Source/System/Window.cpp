#include <EnginePch.h>
#include <System/Window.h>

Window::Window(int width, int height) noexcept
{
	m_pWindow = glfwCreateWindow(width, height, "Lilla Spel", nullptr, nullptr);
	if (m_pWindow == nullptr)
	{
		std::cout << "Failed to create window" << std::endl;
	}
	else
	{
		m_Width = width;
		m_Height = height;
	}

	glfwMakeContextCurrent(m_pWindow);
}


Window::~Window()
{
}


void Window::PollEvents() noexcept
{
	glfwPollEvents();
}


void Window::SwapBuffers() noexcept
{
	glfwSwapBuffers(m_pWindow);
}

bool Window::IsClosed() noexcept
{
	return (glfwWindowShouldClose(m_pWindow) != 0);
}
