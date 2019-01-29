#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>
#include <Graphics/GUI/IRealTimeRendered.h>

class API Slider : public GUIObject, public IMouseListener, public IRealTimeRendered
{
public:
	Slider(float x, float y, float width, float height, Texture2D* textureBackground, Texture2D* textureForeground);
	virtual ~Slider();

	bool isVertical() const noexcept;
	
	void SetForgroundTexture(Texture2D* texture);
	Texture2D* GetForegroundTexture() const;

	void SetRatio(float ratio);
	float GetRatio() const noexcept;

	void SetPercentage(float percentage);
	float GetPercentage() const noexcept;

	virtual void OnAdded(GUIObject* parent) override;
	virtual void OnRemoved(GUIObject* parent) override;

	void OnMousePressed(const glm::vec2& position, MouseButton mousebutton) override;
	void OnMouseReleased(const glm::vec2& position, MouseButton mousebutton) override;
	void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) override;

	void RenderRealTime(GUIContext* context) override;

private:
	Texture2D* m_pTextureForeground;
	bool m_IsPressed;
	float m_Offset;
	float m_Ratio;
	float m_Percentage;
};