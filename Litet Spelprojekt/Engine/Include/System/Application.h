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

	virtual void OnMouseMove(const glm::vec2& position);
	virtual void OnKeyUp(KEY keycode);
	virtual void OnKeyDown(KEY keycode);
	virtual void OnResize(uint32 width, uint32 height);

	Window& GetWindow();
	GLContext& GetContext();

	int32 GetFPS() const noexcept;
	int32 GetUPS() const noexcept;

private:
	Window* m_pWindow;
	GLContext* m_pContext;
	int32 m_fps;
	int32 m_ups;

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

inline int32 Application::GetFPS() const noexcept
{
	return m_fps;
}

inline int32 Application::GetUPS() const noexcept
{
	return m_ups;
}