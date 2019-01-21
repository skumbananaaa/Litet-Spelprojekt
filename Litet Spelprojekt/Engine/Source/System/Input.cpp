#include <EnginePch.h>
#include <System/Input.h>
#include <System/Application.h>

bool g_CurrentKeyState[KEY_COUNT];
bool g_LastKeyState[KEY_COUNT];

bool Input::IsKeyDown(KEY keycode)
{
	return g_CurrentKeyState[keycode];
}

bool Input::IsKeyUp(KEY keycode)
{
	return g_CurrentKeyState[keycode];
}

void Input::Update()
{
	memcpy(g_LastKeyState, g_CurrentKeyState, sizeof(g_CurrentKeyState));
}

void Input::KeyState(KEY keycode, bool state)
{
	g_CurrentKeyState[keycode] = state;
}
