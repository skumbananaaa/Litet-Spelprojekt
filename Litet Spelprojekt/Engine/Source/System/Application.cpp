#include <EnginePch.h>
#include <System/Application.h>
#include <System/Input.h>

constexpr float timestep = 1.0f / 60.0f;
Application* Application::s_Instance = nullptr;

Application::Application(bool fullscreen, uint32 width, uint32 height, const std::string& prePath, bool useMultiThreading)
	: m_pWindow(nullptr),
	m_pGraphicsContext(nullptr),
	m_pGUIManager(nullptr),
	m_fps(0),
	m_ups(0),
	m_ShouldRun(true),
	m_ForcedUpdate(false),
	m_ResourceMode(RESOURCE_MODE::LOAD),
	m_Resource(""),
	m_Progress(0),
	m_LastProgress(0)
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
		m_pWindow = new Window("Krisim", width, height, fullscreen);
		m_pGraphicsContext = new GLContext((float)width, (float)height);
		m_pAudioContext = IAudioContext::CreateContext();
		m_pAudioContext->SetDistanceModel(DistanceModel::LINEAR_DISTANCE);

		ThreadHandler::Init();
		ResourceHandler::LoadResources(this, prePath, useMultiThreading);
		m_pGUIManager = new GUIManager(m_pGraphicsContext);
	}

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

void Application::OnLoading(const std::string& file, float percentage)
{
	m_Resource = file;
	m_Progress = percentage;
}

void Application::OnResourceLoadingFinished()
{
	m_ResourceMode = RESOURCE_MODE::CONSTRUCT;
	std::cout << "OnResourceLoadingFinished()" << std::endl;
}

void Application::DisableRenderingAndForceUpdate(bool yes) noexcept
{
	m_ForcedUpdate = yes;
}

bool Application::IsRenderingDisabled() noexcept
{
	return m_ForcedUpdate;
}

void Application::Exit() noexcept
{
	m_ShouldRun = false;
}

void Application::ResetPrevTime() noexcept
{
	m_PrevTime = std::chrono::high_resolution_clock::now();
}

int32_t Application::Run()
{
	using namespace std;

	using clock = std::chrono::high_resolution_clock;
	using duration = std::chrono::duration<float>;

	auto currentTime = clock::now();
	m_PrevTime = clock::now();
	float deltaTime = 0.0f;
	float totalTime = 0.0f;
	float m_Accumulator = 0.0F;
	int32 fps = 0;
	int32 ups = 0;

	m_pGraphicsContext->SetClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	while (!m_pWindow->IsClosed() && m_ShouldRun)
	{
		if (m_ResourceMode == RESOURCE_MODE::CONSTRUCT)
		{
			ResourceHandler::ConstructResources();
			m_ResourceMode = RESOURCE_MODE::DONE;
			OnResourcesLoaded();
		}
		if (m_LastProgress != m_Progress)
		{
			m_LastProgress = m_Progress;
			OnResourceLoading(m_Resource, m_Progress);
		}
		Input::Update();

		m_pWindow->PollEvents();
		m_pGraphicsContext->Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

		currentTime = clock::now();
		deltaTime = std::chrono::duration_cast<duration>(currentTime - m_PrevTime).count();
		m_PrevTime = currentTime;
		totalTime += deltaTime;

		if (totalTime > 1.0f)
		{
			std::string title = "Krisim [FPS: " + std::to_string(fps) + "] [UPS: " + std::to_string(ups) + ']';

			m_pWindow->SetTitle(title.c_str());

			this->m_fps = fps;
			this->m_ups = ups;
			ups = 0;
			fps = 0;
			totalTime = 0.0f;
		}

		if (m_ForcedUpdate)
		{
			InternalOnUpdate(timestep);
			ups++;
		}
		else
		{
			m_Accumulator += deltaTime;
			while (m_Accumulator > timestep)
			{
				InternalOnUpdate(timestep);
				m_Accumulator -= timestep;
				ups++;
			}

			InternalOnRender(deltaTime);
			fps++;

			m_pWindow->SwapBuffers();
		}
	}

	ThreadHandler::Exit();
	ResourceHandler::ReleaseResources();
	return 0;
}