#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/Button.h>

class API PanelExpandable : public Button
{
public:
	PanelExpandable(float x, float y, float width, float height, float clientHeight, const std::string& text);
	virtual ~PanelExpandable();

	void SetExpanded(bool expanded) noexcept;
	bool IsExpanded() const noexcept;

	float GetClientWidth() const noexcept;
	float GetClientHeight() const noexcept;

	virtual float GetYInWorld(const GUIObject* child = nullptr) const noexcept;

	void SetClientSize(float height) noexcept;

	void RenderChildrensFrameBuffers(GUIContext* context) override;
	void RenderRealTime(GUIContext* context) override;

	virtual bool ContainsPoint(const glm::vec2& position, const GUIObject* caller) const noexcept override;

protected:
	virtual void OnReleased(const glm::vec2& position, MouseButton mousebutton) noexcept override;
	virtual void OnUpdate(float dtS) override;
	virtual void PrintName() const override;

private:
	Framebuffer* m_pFrameBufferClientArea;
	enum MODE
	{
		OPEN,
		CLOSED,
		EXPANDING,
		COLLAPSING,
	};

	MODE m_Mode;
	float m_Percentage;
};