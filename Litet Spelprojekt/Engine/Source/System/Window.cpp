#include <EnginePch.h>
#include <System/Window.h>
#include <System/Application.h>

Window* Window::s_pMainWindow = nullptr;

KEY g_KeyTable[GLFW_KEY_LAST + 1] = 
{
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_SPACE,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_QUOTE,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_COMMA,
	KEY_MINUS,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_UNKNOWN,
	KEY_SEMI_COLON,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_LEFT_BRACKET,
	KEY_BACKSLASH,
	KEY_RIGHT_BRACKET,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //100
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //110
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //120
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,//130
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //140
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //150
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //160
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //170
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //180
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,//190
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,//200
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //210
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //220
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //230
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //240
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN, //250
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_ESCAPE,
	KEY_ENTER,
	KEY_TAB,
	KEY_BACKSPACE,
	KEY_INSERT,
	KEY_DELETE,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_DOWN,
	KEY_UP,
	KEY_PAGE_UP,
	KEY_PAGE_DOWN,
	KEY_HOME,
	KEY_END,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_CAPSLOCK,
	KEY_SCROLLOCK,
	KEY_NUMLOCK,
	KEY_UNKNOWN,
	KEY_PAUSE,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_F13,
	KEY_F14,
	KEY_F15,
	KEY_F16,
	KEY_F17,
	KEY_F18,
	KEY_F19,
	KEY_F20,
	KEY_F21,
	KEY_F22,
	KEY_F23,
	KEY_F24,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_NUMPAD_0,
	KEY_NUMPAD_1,
	KEY_NUMPAD_2,
	KEY_NUMPAD_3,
	KEY_NUMPAD_4,
	KEY_NUMPAD_5,
	KEY_NUMPAD_6,
	KEY_NUMPAD_7,
	KEY_NUMPAD_8,
	KEY_NUMPAD_9,
	KEY_DECIMAL,
	KEY_DIVIDE,
	KEY_MULTIPLY,
	KEY_SUBTRACT,
	KEY_ADD,
	KEY_ENTER,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_UNKNOWN,
	KEY_LEFT_SHIFT,
	KEY_LEFT_CTRL,
	KEY_LEFT_ALT,
	KEY_LEFT_CMD,
	KEY_RIGHT_SHIFT,
	KEY_RIGHT_CTRL,
	KEY_ALTGR,
	KEY_RIGHT_CMD,
	KEY_UNKNOWN,
};

KEY GLFWToKey(int32 key)
{
	return g_KeyTable[key];
}

void ErrorCallback(int32 error, const char* description)
{
	std::cout << "GLFW Error: " << description << std::endl;
}

void Window::KeyCallback(GLFWwindow* pWindow, int32 key, int32 scancode, int32 action, int32 mods)
{
	if (action == GLFW_PRESS)
		Application::GetInstance().InternalOnKeyDown(GLFWToKey(key));
	else if (action == GLFW_RELEASE)
		Application::GetInstance().InternalOnKeyUp(GLFWToKey(key));
}

void Window::MouseMoveCallback(GLFWwindow* pWindow, double x, double y)
{
	s_pMainWindow->m_LastMousePosition = glm::vec2(x, y);
	Application::GetInstance().InternalOnMouseMove(s_pMainWindow->m_LastMousePosition);
}

void Window::MouseButtonCallback(GLFWwindow* pWindow, int32 button, int32 action, int32 mods)
{
	Application::GetInstance().InternalOnMouseButton((MouseButton)button, action == GLFW_PRESS, s_pMainWindow->m_LastMousePosition);
}

void Window::MouseScrollCallback(GLFWwindow* pWindow, double offsetX, double offsetY)
{
	Application::GetInstance().InternalOnMouseScroll(glm::vec2(offsetX, offsetY));
}

void Window::ResizeCallback(GLFWwindow* pWindow, int32 width, int32 height)
{
	Window* pMyWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(pWindow));
	pMyWindow->m_Height = height;
	pMyWindow->m_Width = width;

	Application::GetInstance().InternalOnResize(width, height);
}

Window::Window(const char* pTitle, int32 width, int32 height, bool fullscreen) noexcept
	: m_pWindow(nullptr),
	m_Width(0),
	m_Height(0)
{
	assert(s_pMainWindow == nullptr);
	s_pMainWindow = this;

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	//glfwWindowHint(GLFW_SAMPLES, samples);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(_DEBUG)
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#else
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#endif

	GLFWmonitor* pMonitor = nullptr;
	if (fullscreen)
	{
		pMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(pMonitor);

		width = mode->width;
		height = mode->height;
	}

	m_pWindow = glfwCreateWindow(width, height, pTitle, pMonitor, nullptr);
	if (m_pWindow == nullptr)
	{
		std::cout << "Failed to create window" << std::endl;
	}
	else
	{
		m_Width = width;
		m_Height = height;
	}

	glfwMakeContextCurrent(m_pWindow);
	glfwSwapInterval(0);

	glfwSetErrorCallback(ErrorCallback);
	glfwSetKeyCallback(m_pWindow, KeyCallback);
	glfwSetCursorPosCallback(m_pWindow, MouseMoveCallback);
	glfwSetMouseButtonCallback(m_pWindow, MouseButtonCallback);
	glfwSetWindowSizeCallback(m_pWindow, ResizeCallback);
	glfwSetScrollCallback(m_pWindow, MouseScrollCallback);

	glfwSetWindowUserPointer(m_pWindow, this);
}

Window::~Window()
{
	glfwDestroyWindow(m_pWindow);
}

void Window::SetTitle(const char* pTitle) noexcept
{
	glfwSetWindowTitle(m_pWindow, pTitle);
}

void Window::PollEvents() noexcept
{
	glfwPollEvents();
}

void Window::SwapBuffers() noexcept
{
	glfwSwapBuffers(m_pWindow);
}

bool Window::IsClosed() noexcept
{
	return (glfwWindowShouldClose(m_pWindow) != 0);
}

Window& Window::GetCurrentWindow() noexcept
{
	assert(s_pMainWindow != nullptr);
	return *s_pMainWindow;
}