#include <EnginePch.h>
#include <chrono>
#include <System/Application.h>

using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(16ms);
constexpr float timestepCount = timestep.count();

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
		m_pWindow = new Window(1024, 768);
		m_pContext = new GLRenderer();
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

	if (m_pRenderer != nullptr)
	{
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	glfwTerminate();

	std::cout << "Nudes deleted" << std::endl;
}

int32_t Application::Run()
{
	using clock = std::chrono::high_resolution_clock;
	using timepoint = std::chrono::time_point<std::chrono::steady_clock>;
	using duration = std::chrono::duration<float>;

	timepoint currentTime;
	timepoint prevTime;
	duration deltaTime;
	duration accumulator(0ns);

	while (!m_pWindow->isClosed())
	{
		currentTime = clock::now();
		deltaTime = currentTime - prevTime;
		prevTime = currentTime;

		accumulator += deltaTime;

		while (accumulator > timestep)
		{
			OnUpdate(timestepCount);
			accumulator -= timestep;
		}

		OnRender();
	}

	return 0;
}

Application& Application::GetInstance()
{
	assert(s_Instance != nullptr);
	return *s_Instance;
}
