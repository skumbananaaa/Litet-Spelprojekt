#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>
#include <Graphics/GUI/IRealTimeRendered.h>

class Slider;

class API ISliderListener
{
public:
	virtual void OnSliderChange(Slider* slider, float percentage) = 0;
};

class API Slider : public GUIObject, public IMouseListener, public IRealTimeRendered
{
public:
	Slider(float x, float y, float width, float height, Texture2D* textureBackground, Texture2D* textureForeground, void(*onChangedCallback)(Slider*, float) = nullptr);
	virtual ~Slider();

	bool isVertical() const noexcept;
	
	void SetForgroundTexture(Texture2D* texture);
	Texture2D* GetForegroundTexture() const;

	void SetRatio(float ratio);
	float GetRatio() const noexcept;

	void SetPercentage(float percentage);
	float GetPercentage() const noexcept;

	void AddSliderListener(ISliderListener* listener);
	void RemoveSliderListener(ISliderListener* listener);

	void SetOnSliderChanged(void(*callback)(Slider*, float));

	virtual void OnAdded(GUIObject* parent) override;
	virtual void OnRemoved(GUIObject* parent) override;

	void OnMousePressed(const glm::vec2& position, MouseButton mousebutton) override;
	void OnMouseReleased(const glm::vec2& position, MouseButton mousebutton) override;
	void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) override;

	void RenderRealTime(GUIContext* context) override;

private:
	Texture2D* m_pTextureForeground;
	bool m_IsPressed;
	float m_MouseOffset;
	float m_SliderPos;
	float m_Ratio;
	std::vector<ISliderListener*> m_SliderListeners;
	void(*m_OnChangedCallback)(Slider*, float);
};