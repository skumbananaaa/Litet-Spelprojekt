#pragma once
#include <System\Window.h>
#include <Graphics\Renderers\GLContext.h>
#include <Graphics/GUI/GUIManager.h>
#include <Audio\IAudioContext.h>
#include "Input.h"

class API Application
{
	friend class Window;

public:
	Application(Application&& other) = delete;
	Application(const Application& other) = delete;
	Application& operator=(Application&& other) = delete;
	Application& operator=(const Application& other) = delete;

	Application();
	virtual ~Application();

	int32_t Run();

	Window& GetWindow();
	GLContext& GetGraphicsContext();
	GUIManager& GetGUIManager();

	int32 GetFPS() const noexcept;
	int32 GetUPS() const noexcept;

protected:
	virtual void OnUpdate(float dtS) {};
	virtual void OnRender(float dtS) {};
	virtual void OnMouseMove(const glm::vec2& position) {};
	virtual void OnMousePressed(MouseButton mousebutton) {};
	virtual void OnMouseReleased(MouseButton mousebutton) {};
	virtual void OnKeyUp(KEY keycode) {};
	virtual void OnKeyDown(KEY keycode) {};
	virtual void OnResize(uint32 width, uint32 height) {};

private:
	Window* m_pWindow;
	GLContext* m_pGraphicsContext;
	GUIManager* m_pGUIManager;
	IAudioContext* m_pAudioContext;
	int32 m_fps;
	int32 m_ups;
	bool m_ShouldRun;

	void InternalOnRender(float dtS);
	void InternalOnUpdate(float dtS);
	void InternalOnMouseMove(const glm::vec2& position);
	void InternalOnMouseButton(MouseButton mousebutton, bool down);
	void InternalOnKeyUp(KEY keycode);
	void InternalOnKeyDown(KEY keycode);
	void InternalOnResize(uint32 width, uint32 height);

public:
	static Application& GetInstance();

private:
	static Application* s_Instance;
};


inline void Application::InternalOnRender(float dtS)
{
	OnRender(dtS);
	m_pGUIManager->OnRender();
}

inline void Application::InternalOnUpdate(float dtS)
{
	m_pGUIManager->InternalRootOnUpdate(dtS);
	OnUpdate(dtS);
}

inline void Application::InternalOnMouseMove(const glm::vec2& position)
{
	OnMouseMove(position);
}

inline void Application::InternalOnMouseButton(MouseButton mousebutton, bool down)
{
	if (Input::ButtonState(mousebutton, down))
	{
		if (down)
		{
			OnMousePressed(mousebutton);
			m_pGUIManager->OnMousePressed(mousebutton);
			std::cout << "Mousebutton " << mousebutton << " Pressed" << std::endl;
		}
		else
		{
			OnMouseReleased(mousebutton);
			m_pGUIManager->OnMouseReleased(mousebutton);
			std::cout << "Mousebutton " << mousebutton << " Released" << std::endl;
		}
	}
}

inline void Application::InternalOnKeyUp(KEY keycode)
{
	Input::KeyState(keycode, false);
	OnKeyUp(keycode);
}

inline void Application::InternalOnKeyDown(KEY keycode)
{
	if (keycode == KEY_ESCAPE)
	{
		m_ShouldRun = false;
	}

	Input::KeyState(keycode, true);
	OnKeyDown(keycode);
}

inline void Application::InternalOnResize(uint32 width, uint32 height)
{
	m_pGraphicsContext->SetViewport(width, height, 0, 0);
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

inline GLContext& Application::GetGraphicsContext()
{
	assert(m_pGraphicsContext != nullptr);
	return *m_pGraphicsContext;
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