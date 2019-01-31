#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Slider.h>

class API PanelScrollable : public Panel, public ISliderListener
{
public:
	PanelScrollable(float x, float y, float width, float height, float clientWidth, float clientHeight);
	virtual ~PanelScrollable();

	float GetClientWidth() const noexcept;
	float GetClientHeight() const noexcept;

	void SetClientSize(float width, float height);

	virtual float GetXInWorld(const GUIObject* child = nullptr) const noexcept;
	virtual float GetYInWorld(const GUIObject* child = nullptr) const noexcept;

	virtual void OnAdded(GUIObject* parent) override;
	virtual void OnRemoved(GUIObject* parent) override;

	void OnSliderChange(Slider* slider, float percentage) override;
	virtual bool ContainsPoint(const glm::vec2& position) const noexcept override;

	void RenderChildrensFrameBuffers(GUIContext* context) override;
	void RenderRealTime(GUIContext* context) override;
	virtual void ControllRealTimeRenderingForChildPre(GUIContext* context, GUIObject* child) override;
	virtual void ControllRealTimeRenderingForChildPost(GUIContext* context, GUIObject* child) override;

	virtual void OnMouseScroll(const glm::vec2& position, const glm::vec2& offset) override;

protected:
	virtual void PrintName() const override;

private:
	Slider* m_pSliderVertical;
	Slider* m_pSliderHorizontal;
	Framebuffer* m_pFrameBufferClientArea;
	glm::vec2 m_ClientOffset;
};