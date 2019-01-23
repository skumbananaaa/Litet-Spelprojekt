#include <EnginePch.h>
#include <System/Application.h>
#include <System/Input.h>

constexpr float timestep = 1.0f / 60.0f;
Application* Application::s_Instance = nullptr;

Application::Application()
	: m_pWindow(nullptr), m_pContext(nullptr)
{
	std::cout << "Application" << std::endl;

	assert(s_Instance == nullptr);
	s_Instance = this;

	//Assumes only one window in app
	if (!glfwInit())
	{
		std::cout << "Failed to init GLFW" << std::endl;
	}
	else
	{
		m_pWindow = new Window("Little HOMO", 1024, 768, 1);
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
	float totalTime = 0.0f;
	float accumulator = 0.0f;

	int32 fps = 0;
	int32 ups = 0;

	m_pContext->SetClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	while (!m_pWindow->IsClosed())
	{
		Input::Update();

		m_pWindow->PollEvents();
		m_pContext->Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH | CLEAR_FLAG_STENCIL);

		currentTime = clock::now();
		deltaTime = std::chrono::duration_cast<duration>(currentTime - prevTime).count();
		prevTime = currentTime;
		totalTime += deltaTime;

		if (totalTime > 1.0f)
		{
			std::string title = "Little HOBO [FPS: " + std::to_string(fps) + "] [UPS: " + std::to_string(ups) + ']';
			m_pWindow->SetTitle(title.c_str());

			ups = 0;
			fps = 0;
			totalTime = 0.0f;
		}

		accumulator += deltaTime;
		while (accumulator > timestep)
		{
			OnUpdate(timestep);
			accumulator -= timestep;

			ups++;
		}

		OnRender();
		fps++;

		m_pWindow->SwapBuffers();
	}

	return 0;
}

void Application::OnKeyDown(KEY keycode)
{
	Input::KeyState(keycode, true);
}

void Application::OnMouseMove(const glm::vec2& position)
{
}

void Application::OnKeyUp(KEY keycode)
{
	Input::KeyState(keycode, false);
}

void Application::OnResize(uint32 width, uint32 height)
{
	m_pContext->SetViewport(width, height, 0, 0);
}
