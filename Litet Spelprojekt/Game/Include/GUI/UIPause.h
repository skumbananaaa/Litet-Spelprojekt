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
	Button* m_pButtonTutorial;
	Button* m_pButtonNext;
	Button* m_pButtonPrev;
	Panel* m_pMenuPanel;
	Panel* m_pTutorialPanel;

	int32 m_TutorialScreenIndex;
	std::vector<uint32> m_TutorialScreens;
};
