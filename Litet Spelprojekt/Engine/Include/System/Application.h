#pragma once
#include <System\Window.h>
#include <Graphics\Renderers\GLContext.h>
#include "Input.h"

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

	virtual void OnKeyUp(KEY keycode);
	virtual void OnKeyDown(KEY keycode);
	virtual void OnResize(unsigned int width, unsigned int height);

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

inline Application& Application::GetInstance()
{
	assert(s_Instance != nullptr);
	return *s_Instance;
}

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