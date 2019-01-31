#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>

class Slider;

class API ISliderListener
{
public:
	virtual void OnSliderChange(Slider* slider, float percentage) = 0;
};

class API Slider : public GUIObject
{
public:
	Slider(float x, float y, float width, float height, void(*onChangedCallback)(Slider*, float) = nullptr);
	virtual ~Slider();

	bool IsVertical() const noexcept;

	void SetRatio(float ratio);
	float GetRatio() const noexcept;

	void SetPercentage(float percentage);
	float GetPercentage() const noexcept;

	const glm::vec4& GetSliderColor() const noexcept;
	void SetSliderColor(const glm::vec4& color);

	const glm::vec4& GetOnPressedColor() const noexcept;
	void SetOnPressedColor(const glm::vec4& color);

	const glm::vec4& GetOnHoverColor() const noexcept;
	void SetOnHoverColor(const glm::vec4& color);

	void AddSliderListener(ISliderListener* listener);
	void RemoveSliderListener(ISliderListener* listener);

	void SetOnSliderChanged(void(*callback)(Slider*, float));

	void MoveSlider(float offset);

protected:
	virtual void PrintName() const override;
	virtual const glm::vec4& GetSliderClearColor() const;

	virtual void OnAdded(GUIObject* parent) override;
	virtual void OnRemoved(GUIObject* parent) override;

	virtual void OnMousePressed(const glm::vec2& position, MouseButton mousebutton) override;
	virtual void OnMouseReleased(const glm::vec2& position, MouseButton mousebutton) override;
	virtual void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) override;
	virtual void OnMouseScroll(const glm::vec2& position, const glm::vec2& offset) override;

	void RenderRealTime(GUIContext* context) override;

private:
	Texture2D* m_pTextureForeground;
	bool m_IsPressed;
	bool m_IsHovered;
	float m_MouseOffset;
	float m_SliderPos;
	float m_Ratio;
	glm::vec4 m_SliderColor;
	glm::vec4 m_PressedColor;
	glm::vec4 m_HoverColor;
	std::vector<ISliderListener*> m_SliderListeners;
	void(*m_OnChangedCallback)(Slider*, float);
};