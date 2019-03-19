#pragma once
#include <Graphics/GUI/ProgressBar.h>

class UIReplay : public ProgressBar
{
public:
	UIReplay(float x, float y, float width, float height, float percentage);
	virtual ~UIReplay();

protected:
	virtual void PrintName() const override;
	virtual void RenderProgress(GUIContext* context, float x, float y) noexcept override;
	virtual void OnAdded(GUIObject* parent) override;
	virtual void OnRemoved(GUIObject* parent) override;
	virtual void OnUpdate(float dtS) override;
	virtual void OnMousePressed(const glm::vec2& position, MouseButton mousebutton) override;

private:
	bool m_IsFastForwarding;
};