#pragma once
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/Button.h>

class UIPause : public Panel, public IButtonListener
{
public:
	UIPause(float x, float y, float width, float height);
	virtual ~UIPause();

	virtual void OnButtonPressed(Button* button) override;
	virtual void OnButtonReleased(Button* button) override;
	virtual void OnButtonHovered(Button* button) override;
	virtual void OnButtonNotHovered(Button* button) override;

protected:
	virtual void PrintName() const override;

private:
	TextView* m_pTextViewTitle;
	Button* m_pButtonBack;
	Button* m_pButtonExit;
};
