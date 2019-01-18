#pragma once
#include "Window.h"

class API Application
{
public:
	Application(Application&& other) = delete;
	Application(const Application& other) = delete;
	Application& operator=(Application&& other) = delete;
	Application& operator=(const Application& other) = delete;

	Application();
	~Application();

private:
	Window* m_pWindow;

public:
	static Application& GetInstance();

private:
	static Application* s_Instance;
};