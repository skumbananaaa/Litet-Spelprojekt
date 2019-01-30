#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>

enum TextAlignment
{
	CENTER = 0,
	CENTER_VERTICAL,
	CENTER_HORIZONTAL,
};

class API TextView : public GUIObject
{
public:
	TextView(float x, float y, float width, float height, const std::string& text, int size = 50);
	virtual ~TextView();

	void SetText(const std::string& text);
	const std::string& GetText() const;

	void SetTextSize(int size);
	int GetTextSize() const;

	void SetTextAlignment(TextAlignment textAlignment);
	TextAlignment GetTextAlignment();

protected:
	virtual void OnRender(GUIContext* context) override;
	virtual void RenderText(GUIContext* context);
	virtual void PrintName() const override;

private:
	std::string m_Text;
	int m_TextSize;
	TextAlignment m_TextAlignment;
};