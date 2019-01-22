#pragma once
#include <EnginePch.h>
#include <Defines.h>

struct GLFWwindow;

class API Window
{
public:
	Window(Window&& other) = delete;
	Window(const Window& other) = delete;
	Window& operator=(Window&& other) = delete;
	Window& operator=(const Window& other) = delete;

	Window(const char* pTitle, int width, int height, int samples) noexcept;
	~Window();

	bool IsClosed() noexcept;
	void PollEvents() noexcept;
	void SwapBuffers() noexcept;
	int GetWidth() const noexcept;
	int GetHeight() const noexcept;
	GLFWwindow* GetHandle() const noexcept;

private:
	GLFWwindow* m_pWindow;
	int m_Width;
	int m_Height;

private:
	static Window* s_pMainWindow;

public:
	static Window& GetCurrentWindow() noexcept;
};

inline GLFWwindow* Window::GetHandle() const noexcept
{
	return m_pWindow;
}

inline int Window::GetWidth() const noexcept
{
	return m_Width;
}

inline int Window::GetHeight() const noexcept
{
	return m_Height;
}