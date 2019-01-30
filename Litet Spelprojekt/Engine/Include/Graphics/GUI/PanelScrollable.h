#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Slider.h>

class API PanelScrollable : public Panel, public ISliderListener, public IRealTimeRendered
{
public:
	PanelScrollable(float x, float y, float width, float height, float clientWidth, float clientHeight, Texture2D* texture, Texture2D* sliderBackground, Texture2D* sliderForeground);
	virtual ~PanelScrollable();

	float GetClientWidth() const noexcept;
	float GetClientHeight() const noexcept;

	void SetClientSize(float width, float height);

	virtual void OnAdded(GUIObject* parent) override;
	virtual void OnRemoved(GUIObject* parent) override;

	void OnSliderChange(Slider* slider, float percentage) override;

	void RenderChildrensFrameBuffers(GUIContext* context) override;
	void RenderRealTime(GUIContext* context) override;

private:
	Slider* m_pSliderVertical;
	Slider* m_pSliderHorizontal;
	Framebuffer* m_pFrameBufferClientArea;
	glm::vec2 m_ClientOffset;
};