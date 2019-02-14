#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/PanelExpandable.h>
#include "../Crewmember.h"
#include <Graphics/GUI/SelectionHandler.h>

class UICrew : public IExpandableListener, public ISelectionListener, public IExternalUIRenderer, public IButtonListener
{
public:
	UICrew(float x, float y, float width, float height, const std::vector<Crewmember*> crewmembers);
	virtual ~UICrew();

	virtual void OnExpanding(PanelExpandable* panel, float percentage) override;
	virtual void OnCollapsing(PanelExpandable* panel, float percentage) override;

	virtual void OnSelected(const SelectionHandler* handler, ISelectable* selection) override;
	virtual void OnDeselected(const SelectionHandler* handler, ISelectable* selection) override;

	virtual void OnRenderGUIObject(GUIContext* context, GUIObject* object) override;

	virtual void OnButtonPressed(Button* button) override;
	virtual void OnButtonReleased(Button* button) override;
	virtual void OnButtonHovered(Button* button) override;
	virtual void OnButtonNotHovered(Button* button) override;

private:
	PanelExpandable* m_Fires;
	PanelExpandable* m_Medics;
	PanelExpandable* m_Strengths;
	SelectionHandler m_SelectionHandler;
};
