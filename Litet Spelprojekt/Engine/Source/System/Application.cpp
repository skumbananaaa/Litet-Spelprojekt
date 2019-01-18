#include <EnginePch.h>
#include <System/Application.h>

Application* Application::s_Instance = nullptr;

Application::Application()
	: m_pWindow(nullptr)
{
	assert(s_Instance == nullptr);
	s_Instance = this;

	std::cout << "Send Nudes" << std::endl;
}

Application::~Application()
{
	if (m_pWindow != nullptr)
		delete m_pWindow;

	std::cout << "Nudes deleted" << std::endl;
}

Application& Application::GetInstance()
{
	assert(s_Instance != nullptr);
	return *s_Instance;
}
