#pragma once
#include <Defines.h>

class API Window
{
public:
	Window(Window&& other) = delete;
	Window(const Window& other) = delete;
	Window& operator=(Window&& other) = delete;
	Window& operator=(const Window& other) = delete;

	Window(int width, int height);
	~Window();

	void PollEvents();

private:
	GLFWwindow* m_pWindow;
};