#include <EnginePch.h>
#include <System/Input.h>
#include <System/Application.h>

bool g_CurrentKeyState[KEY_COUNT];
bool g_LastKeyState[KEY_COUNT];
glm::vec2 g_MousePosition = glm::vec2();

bool Input::IsKeyDown(KEY keycode)
{
	return g_CurrentKeyState[keycode];
}

bool Input::IsKeyUp(KEY keycode)
{
	return g_CurrentKeyState[keycode];
}

const glm::vec2& Input::GetMousePosition()
{
	return g_MousePosition;
}

void Input::SetMousePosition(const glm::vec2& position)
{
	g_MousePosition = position;
	glfwSetCursorPos(Application::GetInstance().GetWindow().GetHandle(), position.x, position.y);
}

void Input::Update()
{
	memcpy(g_LastKeyState, g_CurrentKeyState, sizeof(g_CurrentKeyState));
}

void Input::KeyState(KEY keycode, bool state)
{
	g_CurrentKeyState[keycode] = state;
}

void Input::MousePosition(const glm::vec2& position)
{
	g_MousePosition = position;
}
