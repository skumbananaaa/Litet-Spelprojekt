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

	void SetText(const std::string& text);
	const std::string& GetText() const;

	void SetSize(int size);
	int GetSize() const;

	void SetTextAlignment(TextAlignment textAlignment);
	TextAlignment GetTextAlignment();

protected:
	virtual void OnRender(GUIContext* context) override;

private:
	std::string m_Text;
	int m_Size;
	TextAlignment m_TextAlignment;
};