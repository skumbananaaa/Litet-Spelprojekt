#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>

class API TextView : public GUIObject
{
public:
	TextView(float x, float y, float width, float height, const std::string& text, bool center = false, int size = 55);
	virtual ~TextView();

	void SetText(const std::string& text);
	const std::string& GetText() const;

	void SetTextSize(int size);
	int GetTextSize() const;

	void SetTextCentered(bool center);
	bool IsTextCentered();

	const glm::vec4& GetTextColor() const noexcept;
	void SetTextColor(const glm::vec4& color);

	const glm::vec2& GetTextOffset() const noexcept;
	void SetTextOffset(const glm::vec2& offset);

protected:
	virtual void OnRender(GUIContext* context) override;
	virtual void RenderText(GUIContext* context);
	virtual void PrintName() const override;
	virtual const glm::vec4& GetClearTextColor() const;

private:
	std::string m_Text;
	int m_TextSize;
	bool m_TextCentered;
	glm::vec4 m_TextColor;
	glm::vec2 m_TextOffset;
};