#pragma once
#include <System\Window.h>
#include <Graphics\Renderers\GLContext.h>

class API Application
{
public:
	Application(Application&& other) = delete;
	Application(const Application& other) = delete;
	Application& operator=(Application&& other) = delete;
	Application& operator=(const Application& other) = delete;

	Application();
	~Application();

	int32_t Run();
	virtual void OnUpdate(float dtS) {};
	virtual void OnRender() {};

	Window& GetWindow();
	GLContext& GetContext();

private:
	Window* m_pWindow;
	GLContext* m_pContext;

public:
	static Application& GetInstance();

private:
	static Application* s_Instance;
};

inline Window& Application::GetWindow()
{
	assert(m_pWindow != nullptr);
	return *m_pWindow;
}

inline GLContext& Application::GetContext()
{
	assert(m_pContext != nullptr);
	return *m_pContext;
}