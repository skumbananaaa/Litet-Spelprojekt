#include <EnginePch.h>
#include <System/Application.h>

Application* Application::s_Instance = nullptr;

Application::Application()
	: m_pWindow(nullptr)
{
	assert(s_Instance == nullptr);
	s_Instance = this;

	//Assumes only one window in app
	if (!glfwInit())
	{
		std::cout << "Failed to init GLFW" << std::endl;
	}
	else
	{
		m_pWindow = new Window(1024, 768);
	}
	

	std::cout << "Send Nudes" << std::endl;
}

Application::~Application()
{
	assert(s_Instance == this);
	s_Instance = nullptr;

	if (m_pWindow != nullptr)
	{
		delete m_pWindow;
		m_pWindow = nullptr;
	}

	glfwTerminate();

	std::cout << "Nudes deleted" << std::endl;
}

Application& Application::GetInstance()
{
	assert(s_Instance != nullptr);
	return *s_Instance;
}
