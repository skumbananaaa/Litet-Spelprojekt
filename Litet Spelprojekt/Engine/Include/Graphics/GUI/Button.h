#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/ISelectable.h>

class Button;

class API IButtonListener
{
public:
	virtual void OnButtonPressed(Button* button) = 0;
	virtual void OnButtonReleased(Button* button) = 0;
};


class API Button : public TextView, public ISelectable
{
public:
	Button(float x, float y, float width, float height, const std::string& text, void(*onPressedCallback)(Button*) = nullptr, void(*onReleasedCallback)(Button*) = nullptr, int textSize = 100);
	virtual ~Button();

	Texture2D* GetOnPressedTexture() const noexcept;
	void SetOnPressedTexture(Texture2D* texture);

	const glm::vec4& GetOnPressedColor() const noexcept;
	void SetOnPressedColor(const glm::vec4& color);

	const glm::vec4& GetOnHoverColor() const noexcept;
	void SetOnHoverColor(const glm::vec4& color);

	const glm::vec4& GetOnSelectedColor() const noexcept;
	void SetOnSelectedColor(const glm::vec4& color);

	const glm::vec4& GetOnPressedTextColor() const noexcept;
	void SetOnPressedTextColor(const glm::vec4& color);

	const glm::vec4& GetOnHoverTextColor() const noexcept;
	void SetOnHoverTextColor(const glm::vec4& color);

	const glm::vec4& GetOnSelectedTextColor() const noexcept;
	void SetOnSelectedTextColor(const glm::vec4& color);

	virtual void SetSelected(bool selected) override;

	void AddButtonListener(IButtonListener* listener);
	void RemoveButtonListener(IButtonListener* listener);

	void SetOnButtonPressed(void(*callback)(Button*));
	void SetOnButtonCallback(void(*callback)(Button*));

protected:
	virtual void OnMousePressed(const glm::vec2& position, MouseButton mousebutton) override;
	virtual void OnMouseReleased(const glm::vec2& position, MouseButton mousebutton) override;
	virtual void OnMouseMove(const glm::vec2& position) override;

	virtual void OnAdded(GUIObject* parent) override;
	virtual void OnRemoved(GUIObject* parent) override;

	virtual void PrintName() const override;
	virtual const glm::vec4& GetClearColor() const override;
	virtual Texture2D* GetClearTexture() const override;
	virtual const glm::vec4& GetClearTextColor() const;

private:
	Texture2D* m_pOnPressedTexture;
	bool m_IsPressed;
	bool m_IsHovered;
	glm::vec4 m_PressedColor;
	glm::vec4 m_HoverColor;
	glm::vec4 m_SelectedColor;
	glm::vec4 m_PressedTextColor;
	glm::vec4 m_HoverTextColor;
	glm::vec4 m_SelectedTextColor;
	std::vector<IButtonListener*> m_ButtonListeners;
	void (*m_OnPressedCallback)(Button*);
	void (*m_OnReleasedCallback)(Button*);
};