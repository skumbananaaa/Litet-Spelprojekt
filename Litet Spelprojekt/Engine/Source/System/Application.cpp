#include <EnginePch.h>
#include <System/Application.h>
#include <System/Input.h>

constexpr float timestep = 1.0f / 60.0f;
Application* Application::s_Instance = nullptr;

Application::Application()
	: m_pWindow(nullptr), 
	m_pGraphicsContext(nullptr),
	m_pGUIManager(nullptr),
	m_fps(0), 
	m_ups(0),
	m_ShouldRun(true)
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
		m_pWindow = new Window("Small Game Project", 1920, 1080, false);

		float width = static_cast<float>(m_pWindow->GetWidth());
		float height = static_cast<float>(m_pWindow->GetHeight());

		m_pGraphicsContext = new GLContext(width, height);
		m_pGUIManager = new GUIManager(m_pGraphicsContext);
	}

	m_pAudioContext = IAudioContext::CreateContext();
	
	std::cout << "Application Initalized" << std::endl;
}

Application::~Application()
{
	assert(s_Instance == this);
	s_Instance = nullptr;

	DeleteSafe(m_pWindow);
	DeleteSafe(m_pGraphicsContext);
	DeleteSafe(m_pGUIManager);
	DeleteSafe(m_pAudioContext);

	glfwTerminate();

	std::cout << "Application deleted" << std::endl;
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

	m_pGraphicsContext->SetClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	while (!m_pWindow->IsClosed() && m_ShouldRun)
	{
		Input::Update();

		m_pWindow->PollEvents();
		m_pGraphicsContext->Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

		currentTime = clock::now();
		deltaTime = std::chrono::duration_cast<duration>(currentTime - prevTime).count();
		prevTime = currentTime;
		totalTime += deltaTime;

		if (totalTime > 1.0f)
		{
			std::string title = "Small Game Project [FPS: " + std::to_string(fps) + "] [UPS: " + std::to_string(ups) + ']';
			m_pWindow->SetTitle(title.c_str());

			this->m_fps = fps;
			this->m_ups = ups;
			ups = 0;
			fps = 0;
			totalTime = 0.0f;
		}

		accumulator += deltaTime;
		while (accumulator > timestep)
		{
			InternalOnUpdate(timestep);
			accumulator -= timestep;

			ups++;
		}

		InternalOnRender(deltaTime);
		fps++;

		m_pWindow->SwapBuffers();
	}

	return 0;
}