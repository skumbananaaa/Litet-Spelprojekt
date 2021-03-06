#pragma once
#include <System/Window.h>
#include <Audio/IAudioContext.h>
#include <Graphics/GUI/GUIManager.h>
#include <Graphics/Renderers/GLContext.h>
#include <Graphics/Renderers/DebugRenderer.h>
#include "Input.h"
#include <System/ThreadHandler.h>
#include <IO/ResourceHandler.h>

class API Application : public IResourceListener
{
	friend class Window;

public:
	Application(Application&& other) = delete;
	Application(const Application& other) = delete;
	Application& operator=(Application&& other) = delete;
	Application& operator=(const Application& other) = delete;

	Application(bool fullscreen = true, uint32 width = 1024, uint32 height = 768, const std::string& prePath = "", bool useMultiThreading = true);
	virtual ~Application();

	virtual void OnLoading(const std::string&, float percentage) override;
	virtual void OnResourceLoadingFinished() override;

	int32_t Run();

	Window& GetWindow();
	GLContext& GetGraphicsContext();
	GUIManager& GetGUIManager();

	int32 GetFPS() const noexcept;
	int32 GetUPS() const noexcept;

protected:
	virtual void OnResourceLoading(const std::string& file, float percentage) {};
	virtual void OnResourcesLoaded() {};
	virtual void OnUpdate(float dtS) {};
	virtual void OnUpdateLoading(float dtS) {};
	virtual void OnRender(float dtS) {};
	virtual void OnRenderLoading(float dtS) {};
	virtual void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) {};
	virtual void OnMouseScroll(const glm::vec2& offset, const glm::vec2& position) {};
	virtual void OnMousePressed(MouseButton mousebutton, const glm::vec2& position) {};
	virtual void OnMouseReleased(MouseButton mousebutton, const glm::vec2& position) {};
	virtual void OnKeyUp(KEY keycode) {};
	virtual void OnKeyDown(KEY keycode) {};
	virtual void OnResize(uint32 width, uint32 height) {};

private:
	enum RESOURCE_MODE
	{
		LOAD,
		CONSTRUCT,
		DONE
	};

	Window* m_pWindow;
	GLContext* m_pGraphicsContext;
	GUIManager* m_pGUIManager;
	IAudioContext* m_pAudioContext;
	int32 m_fps;
	int32 m_ups;
	bool m_ShouldRun;
	RESOURCE_MODE m_ResourceMode;
	std::string m_Resource;
	float m_Progress;
	float m_LastProgress;

	void InternalOnRender(float dtS);
	void InternalOnUpdate(float dtS);
	void InternalOnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position);
	void InternalOnMouseScroll(const glm::vec2& offset, const glm::vec2& position);
	void InternalOnMouseButton(MouseButton mousebutton, bool down, const glm::vec2& position);
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
	if (m_ResourceMode == RESOURCE_MODE::DONE)
	{
		OnRender(dtS);
	}
	else
	{
		OnRenderLoading(dtS);
	}
	m_pGUIManager->InternalRootOnRender();
}

inline void Application::InternalOnUpdate(float dtS)
{
	if (m_ResourceMode == RESOURCE_MODE::DONE)
	{
		OnUpdate(dtS);
	}
	else
	{
		OnUpdateLoading(dtS);
	}
	m_pGUIManager->InternalRootOnUpdate(dtS);
}

inline void Application::InternalOnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	if (m_ResourceMode == RESOURCE_MODE::DONE)
	{
		OnMouseMove(lastPosition, position);
		m_pGUIManager->InternalRootOnMouseMove(position);
	}
}

inline void Application::InternalOnMouseScroll(const glm::vec2& offset, const glm::vec2& position)
{
	if (m_ResourceMode == RESOURCE_MODE::DONE)
	{
	    OnMouseScroll(offset, position);
		m_pGUIManager->InternalRootOnMouseScroll(position, offset);
	}
}

inline void Application::InternalOnMouseButton(MouseButton mousebutton, bool down, const glm::vec2& position)
{
	if (m_ResourceMode == RESOURCE_MODE::DONE)
	{
		if (Input::ButtonState(mousebutton, down))
		{
			if (down)
			{
				OnMousePressed(mousebutton, position);
				m_pGUIManager->InternalRootOnMousePressed(position, mousebutton);
			}
			else
			{
				OnMouseReleased(mousebutton, position);
				m_pGUIManager->InternalRootOnMouseReleased(position, mousebutton);
			}
		}
	}
}

inline void Application::InternalOnKeyUp(KEY keycode)
{
	Input::KeyState(keycode, false);
	if (m_ResourceMode == RESOURCE_MODE::DONE)
	{
		OnKeyUp(keycode);
	}
}

inline void Application::InternalOnKeyDown(KEY keycode)
{
	if (keycode == KEY_ESCAPE)
	{
		m_ShouldRun = false;
	}
	Input::KeyState(keycode, true);

	if (m_ResourceMode == RESOURCE_MODE::DONE)
	{
		OnKeyDown(keycode);
	}
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