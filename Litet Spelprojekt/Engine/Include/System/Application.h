#pragma once
#include <System\Window.h>
#include <Graphics\Renderers\GLContext.h>
#include "Input.h"
#include <Graphics/GUI/GUIManager.h>

class API Application
{
	friend class Window;

public:
	Application(Application&& other) = delete;
	Application(const Application& other) = delete;
	Application& operator=(Application&& other) = delete;
	Application& operator=(const Application& other) = delete;

	Application();
	~Application();

	int32_t Run();

	Window& GetWindow();
	GLContext& GetContext();
	GUIManager& GetGUIManager();

	int32 GetFPS() const noexcept;
	int32 GetUPS() const noexcept;

protected:
	virtual void OnUpdate(float dtS) {};
	virtual void OnRender() {};
	virtual void OnMouseMove(const glm::vec2& position) {};
	virtual void OnKeyUp(KEY keycode) {};
	virtual void OnKeyDown(KEY keycode) {};
	virtual void OnResize(uint32 width, uint32 height) {};

private:
	Window* m_pWindow;
	GLContext* m_pContext;
	GUIManager* m_pGUIManager;
	int32 m_fps;
	int32 m_ups;

	void InternalOnRender();
	void InternalOnUpdate(float dtS);
	void InternalOnMouseMove(const glm::vec2& position);
	void InternalOnKeyUp(KEY keycode);
	void InternalOnKeyDown(KEY keycode);
	void InternalOnResize(uint32 width, uint32 height);

public:
	static Application& GetInstance();

private:
	static Application* s_Instance;
};


inline void Application::InternalOnRender()
{
	OnRender();
	m_pGUIManager->OnRender();
}

inline void Application::InternalOnUpdate(float dtS)
{
	OnUpdate(dtS);
	m_pGUIManager->OnUpdate(dtS);
}

inline void Application::InternalOnMouseMove(const glm::vec2 & position)
{

}

inline void Application::InternalOnKeyUp(KEY keycode)
{
	Input::KeyState(keycode, false);
	OnKeyUp(keycode);
}

inline void Application::InternalOnKeyDown(KEY keycode)
{
	Input::KeyState(keycode, true);
	OnKeyDown(keycode);
}

inline void Application::InternalOnResize(uint32 width, uint32 height)
{
	m_pContext->SetViewport(width, height, 0, 0);
	OnResize(width, height);
}

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

inline GUIManager& Application::GetGUIManager()
{
	assert(m_pGUIManager != nullptr);
	return *m_pGUIManager;
}

inline int32 Application::GetFPS() const noexcept
{
	return m_fps;
}

inline int32 Application::GetUPS() const noexcept
{
	return m_ups;
}