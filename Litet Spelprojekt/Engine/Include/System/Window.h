#pragma once
#include <EnginePch.h>
#include <Defines.h>

struct GLFWwindow;

class API Window
{
	friend void ResizeCallback(GLFWwindow*, int32, int32);

public:
	Window(Window&& other) = delete;
	Window(const Window& other) = delete;
	Window& operator=(Window&& other) = delete;
	Window& operator=(const Window& other) = delete;

	Window(const char* pTitle, int32 width, int32 height, int32 samples) noexcept;
	~Window();

	bool IsClosed() noexcept;
	void PollEvents() noexcept;
	void SwapBuffers() noexcept;
	void SetTitle(const char* pTitle) noexcept;
	int32 GetWidth() const noexcept;
	int32 GetHeight() const noexcept;
	GLFWwindow* GetHandle() const noexcept;

private:
	GLFWwindow* m_pWindow;
	int32 m_Width;
	int32 m_Height;

private:
	static Window* s_pMainWindow;

	static void KeyCallback(GLFWwindow* pWindow, int32 key, int32 scancode, int32 action, int32 mods);
	static void MouseMoveCallback(GLFWwindow* pWindow, double x, double y);
	static void MouseButtonCallback(GLFWwindow* pWindow, int32 button, int32 action, int32 mods);
	static void ResizeCallback(GLFWwindow* pWindow, int32 width, int32 height);

public:
	static Window& GetCurrentWindow() noexcept;
};

inline GLFWwindow* Window::GetHandle() const noexcept
{
	return m_pWindow;
}

inline int32 Window::GetWidth() const noexcept
{
	return m_Width;
}

inline int32 Window::GetHeight() const noexcept
{
	return m_Height;
}