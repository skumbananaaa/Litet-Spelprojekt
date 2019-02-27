#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/Button.h>
#include <Graphics/GUI/SelectionHandler.h>

enum TextFieldType : uint8
{
	ALL_CHARACTERS,
	NUMBERS_ONLY,
	LETTERS_ONLY
};

class API TextField : public Button
{
public:
	TextField(float x, float y, float width, float height, TextFieldType type, const std::string& text, int textSize = 100);
	virtual ~TextField();

	virtual uint32 GetTextAsUInt();

protected:
	virtual void OnKeyUp(KEY keycode) override;
	virtual void OnKeyDown(KEY keycode) override;
	virtual void OnCharFromKey(char c) override;

	virtual bool CanPrintChar(char c);

private:
	TextFieldType m_Type;

private:
	static SelectionHandler s_SelectionHandler;
};