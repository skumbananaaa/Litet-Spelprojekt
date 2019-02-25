#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/ListScrollable.h>
#include <Graphics/GUI/SelectionHandler.h>
#include <World/Scenarios/IScenario.h>
#include <Graphics/GUI/Button.h>
#include "../Crewmember.h"

class UIScenario : public Panel, public ISelectionListener
{
public:
	UIScenario(float x, float y, float width, float height);
	virtual ~UIScenario();

	void CreateScenario(IScenario* scenario) noexcept;

	virtual void OnSelected(const SelectionHandler* handler, ISelectable* selection) override;
	virtual void OnDeselected(const SelectionHandler* handler, ISelectable* selection) override;

protected:
	virtual void PrintName() const override;

private:
	TextView* m_pTextViewTitle;
	Button* m_pButtonBack;
	Button* m_pButtonStart;
	ListScrollable* m_pListScrollable;
	SelectionHandler m_SelectionHandler;
};
