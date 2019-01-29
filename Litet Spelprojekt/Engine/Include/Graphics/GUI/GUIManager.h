#pragma once
#include <EnginePch.h>
#include <System/Input.h>
#include <Graphics/Renderers/GLContext.h>
#include <Graphics/GUI/GUIContext.h>
#include <Graphics/GUI/GUIObject.h>

class API GUIManager : public GUIObject
{
public:
	GUIManager(GLContext* glContext);
	~GUIManager();

	void OnRender();
	void InternalRootOnUpdate(float dtS);
	void OnMousePressed(MouseButton mousebutton);
	void OnMouseReleased(MouseButton mousebutton);
	void OnMouseMove(const glm::vec2& position);

	void OnKeyUp(KEY keycode);
	void OnKeyDown(KEY keycode);

private:
	GUIContext* context;
};