#pragma once
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Button.h>

class UICrewRequest : public Panel, public IButtonListener
{
public:
	UICrewRequest(float x, float y, float width, float height);
	virtual ~UICrewRequest();

	virtual void OnButtonPressed(Button* button) override;
	virtual void OnButtonReleased(Button* button) override;
	virtual void OnButtonHovered(Button* button) override;
	virtual void OnButtonNotHovered(Button* button) override;
	
};