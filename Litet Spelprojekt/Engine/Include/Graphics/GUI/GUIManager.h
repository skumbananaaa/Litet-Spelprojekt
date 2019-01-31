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

	virtual float GetWidth() const noexcept override;
	virtual float GetHeight() const noexcept override;

	void InternalRootOnRender();
	void InternalRootOnUpdate(float dtS);
	void InternalRootOnMousePressed(MouseButton mousebutton);
	void InternalRootOnMouseReleased(MouseButton mousebutton);
	void InternalRootOnMouseMove(const glm::vec2& position);
	void InternalRootOnMouseScroll(const glm::vec2& offset);

	void InternalRootOnKeyUp(KEY keycode);
	void InternalRootOnKeyDown(KEY keycode);

protected:
	virtual void PrintName() const override;

private:
	GUIContext* context;
	glm::vec2 m_LastMousePosition;
};