#include <EnginePch.h>
#include <System/Window.h>

Window::Window(const char* pTitle, int width, int height) noexcept
	: m_pWindow(nullptr),
	m_Width(0),
	m_Height(0)
{
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

#if defined(_DEBUG)
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

	m_pWindow = glfwCreateWindow(width, height, pTitle, nullptr, nullptr);
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
	glfwSwapInterval(0);
}


Window::~Window()
{
	glfwDestroyWindow(m_pWindow);
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
