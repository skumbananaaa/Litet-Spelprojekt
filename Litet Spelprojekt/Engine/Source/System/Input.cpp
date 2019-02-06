#include <EnginePch.h>
#include <System/Input.h>
#include <System/Application.h>

bool g_CurrentKeyState[KEY_COUNT];
bool g_LastKeyState[KEY_COUNT];

bool g_CurrentButtonState[3];

bool Input::IsKeyPressed(KEY keycode)
{
	return g_CurrentKeyState[keycode] && !g_LastKeyState[keycode];
}

bool Input::IsKeyDown(KEY keycode)
{
	return g_CurrentKeyState[keycode];
}

bool Input::IsKeyUp(KEY keycode)
{
	return !g_CurrentKeyState[keycode];
}

bool Input::IsButtonDown(MouseButton button)
{
	return g_CurrentButtonState[button];
}

bool Input::IsButtonUp(MouseButton button)
{
	return !g_CurrentButtonState[button];
}

glm::vec2 Input::GetMousePosition()
{
	double x = 0;
	double y = 0;

	glfwGetCursorPos(Window::GetCurrentWindow().GetHandle(), &x, &y);
	return glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

void Input::SetMousePosition(const glm::vec2& position)
{
	glfwSetCursorPos(Window::GetCurrentWindow().GetHandle(), position.x, position.y);
}

void Input::Update()
{
	memcpy(g_LastKeyState, g_CurrentKeyState, sizeof(g_CurrentKeyState));
}

void Input::KeyState(KEY keycode, bool state)
{
	g_CurrentKeyState[keycode] = state;
}

/*
Returns true if the new state has changed
*/
bool Input::ButtonState(MouseButton button, bool state)
{
	if (g_CurrentButtonState[button] != state)
	{
		g_CurrentButtonState[button] = state;
		return true;
	}

	return false;
}
