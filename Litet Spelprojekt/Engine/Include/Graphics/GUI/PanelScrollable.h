#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Slider.h>

class API PanelScrollable : public Panel, public ISliderListener
{
public:
	PanelScrollable(float x, float y, float width, float height, float clientWidth, float clientHeight);
	virtual ~PanelScrollable();

	virtual void SetVisible(bool visible) noexcept override;

	float GetClientWidth() const noexcept;
	float GetClientHeight() const noexcept;

	void SetClientSize(float width, float height);

	virtual float GetXInWorld(const GUIObject* child = nullptr) const noexcept;
	virtual float GetYInWorld(const GUIObject* child = nullptr) const noexcept;

	virtual void OnAdded(GUIObject* parent) override;
	virtual void OnRemoved(GUIObject* parent) override;

	void OnSliderChange(Slider* slider, float percentage) override;
	virtual bool ContainsPoint(const glm::vec2& position, const GUIObject* caller) const noexcept override;

	void RenderChildrensFrameBuffers(GUIContext* context) override;
	virtual void RenderRealTimePre(GUIContext* context, float x = 0, float y = 0) override;
	virtual void RenderRealTime(GUIContext* context, float x = 0, float y = 0) override;
	virtual void RenderRealTimePost(GUIContext* context) override;

	virtual void OnMouseScroll(const glm::vec2& position, const glm::vec2& offset) override;

protected:
	virtual void PrintName() const override;

private:
	Slider* m_pSliderVertical;
	Slider* m_pSliderHorizontal;
	Framebuffer* m_pFrameBufferClientArea;
	glm::vec2 m_ClientOffset;
};