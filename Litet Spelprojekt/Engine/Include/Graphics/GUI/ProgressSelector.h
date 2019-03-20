#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/ProgressBar.h>

class API ProgressSelector : public ProgressBar
{
public:
	ProgressSelector(float x, float y, float width, float height);
	virtual ~ProgressSelector();

	virtual void OnMouseMove(const glm::vec2& position) override;
	virtual void OnMousePressed(const glm::vec2& position, MouseButton mousebutton) override;
	virtual void OnMouseReleased(const glm::vec2& position, MouseButton mousebutton) override;

protected:
	virtual void PrintName() const override;

	virtual void OnAdded(GUIObject* parent) override;
	virtual void OnRemoved(GUIObject* parent) override;

private:
	bool m_IsPressed;
};