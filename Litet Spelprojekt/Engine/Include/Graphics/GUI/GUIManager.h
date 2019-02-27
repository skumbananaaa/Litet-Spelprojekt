#pragma once
#include <EnginePch.h>
#include <System/Input.h>
#include <Graphics/Renderers/GLContext.h>
#include <Graphics/GUI/GUIContext.h>
#include <Graphics/GUI/GUIObject.h>
#include <IO/ResourceHandler.h>

class API GUIManager : public GUIObject
{
public:
	GUIManager(GLContext* glContext);
	~GUIManager();

	virtual float GetWidth() const noexcept override;
	virtual float GetHeight() const noexcept override;

	void InternalRootOnRender();
	void InternalRootOnUpdate(float dtS);
	void InternalRootOnMousePressed(const glm::vec2& position, MouseButton mousebutton);
	void InternalRootOnMouseReleased(const glm::vec2& position, MouseButton mousebutton);
	void InternalRootOnMouseMove(const glm::vec2& position);
	void InternalRootOnMouseScroll(const glm::vec2& position, const glm::vec2& offset);

	void InternalRootOnKeyUp(KEY keycode);
	void InternalRootOnKeyDown(KEY keycode);
	void InternalRootOnCharFromKey(char c);

protected:
	virtual void PrintName() const override;

private:
	GUIContext* context;
};