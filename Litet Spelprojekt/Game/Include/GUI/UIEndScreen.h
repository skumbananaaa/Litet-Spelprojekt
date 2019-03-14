#pragma once
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/Button.h>
#include <Graphics/GUI/ListScrollable.h>

class UIEndScreen : public Panel, public IButtonListener
{
public:
	UIEndScreen(float x, float y, float width, float height, bool lost);
	virtual ~UIEndScreen();

	virtual void OnButtonPressed(Button* button) override;
	virtual void OnButtonReleased(Button* button) override;
	virtual void OnButtonHovered(Button* button) override;
	virtual void OnButtonNotHovered(Button* button) override;

protected:
	virtual void PrintName() const override;

private:
	TextView* m_pTextViewTitle;
	TextView* m_pTextViewEnd;
	TextView* m_pTextViewCrew;
	TextView* m_pTextViewFire;
	TextView* m_pTextViewWater;
	Button* m_pButtonExit;
	ListScrollable* m_pLog;
};
