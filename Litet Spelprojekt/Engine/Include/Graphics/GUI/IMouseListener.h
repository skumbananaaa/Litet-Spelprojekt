#pragma once
#include <EnginePch.h>
#include <System/Input.h>

class API IMouseListener
{
public:
	virtual void OnMousePressed(const glm::vec2& position, MouseButton mousebutton) = 0;
	virtual void OnMouseReleased(const glm::vec2& position, MouseButton mousebutton) = 0;
	virtual void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) = 0;
};