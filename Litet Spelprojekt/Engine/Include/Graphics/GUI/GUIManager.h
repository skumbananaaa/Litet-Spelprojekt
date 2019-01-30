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

	void InternalRootOnRender();
	void InternalRootOnUpdate(float dtS);
	void InternalRootOnMousePressed(MouseButton mousebutton);
	void InternalRootOnMouseReleased(MouseButton mousebutton);
	void InternalRootOnMouseMove(const glm::vec2& position);

	void InternalRootOnKeyUp(KEY keycode);
	void InternalRootOnKeyDown(KEY keycode);

private:
	GUIContext* context;
	glm::vec2 m_LastMousePosition;
};