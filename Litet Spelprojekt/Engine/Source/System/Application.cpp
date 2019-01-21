#include <EnginePch.h>
#include <chrono>
#include <System/Application.h>

constexpr float timestep = 1.0f / 60.0f;
Application* Application::s_Instance = nullptr;

Application::Application()
	: m_pWindow(nullptr), m_pContext(nullptr)
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
		m_pWindow = new Window("Little HOMO", 1024, 768);
		m_pContext = new GLContext();
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

	if (m_pContext != nullptr)
	{
		delete m_pContext;
		m_pContext = nullptr;
	}

	glfwTerminate();

	std::cout << "Nudes deleted" << std::endl;
}

int32_t Application::Run()
{
	using namespace std;

	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<float>;

	auto currentTime = clock::now();
	auto prevTime = clock::now();
	float deltaTime = 0.0f;
	float accumulator = 0.0f;

	m_pContext->SetClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	while (!m_pWindow->IsClosed())
	{
		m_pWindow->PollEvents();
		m_pContext->Clear();

		currentTime = clock::now();
		deltaTime = std::chrono::duration_cast<duration>(currentTime - prevTime).count();
		prevTime = currentTime;
		

		accumulator += deltaTime;
		while (accumulator > timestep)
		{
			OnUpdate(timestep);
			accumulator -= timestep;
		}

		OnRender();
		m_pWindow->SwapBuffers();
	}

	return 0;
}

Application& Application::GetInstance()
{
	assert(s_Instance != nullptr);
	return *s_Instance;
}
