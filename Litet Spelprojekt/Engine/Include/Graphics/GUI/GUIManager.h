#pragma once

class GUIManager
{
	friend class Application;

private:

	virtual void OnUpdate(float dtS) {};
	virtual void OnRender() {};

	/*virtual void OnMouseMove(const glm::vec2& position);
	virtual void OnKeyUp(KEY keycode);
	virtual void OnKeyDown(KEY keycode);*/
};