#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/ISelectable.h>

class SelectionHandler;

class API ISelectionListener
{
public:
	virtual void OnSelected(const SelectionHandler* handler, ISelectable* selection) = 0;
	virtual void OnDeselected(const SelectionHandler* handler, ISelectable* selection) = 0;
};

class API SelectionHandler : public ISelectableListener
{
public:
	SelectionHandler(bool atLeastOne = true);
	~SelectionHandler();

	void AddSelectable(ISelectable* selectable);
	void RemoveSelectable(ISelectable* selectable);
	bool AddSelectionListener(ISelectionListener* listener);
	void RemoveSelectionListener(ISelectionListener* listener);

	virtual void OnSelected(ISelectable* selection) override;
	virtual void OnDeselected(ISelectable* selection) override {};

private:
	virtual void TriggerOnSelected(ISelectable* selection) const;
	virtual void TriggerOnDeselected(ISelectable* selection) const;

private:
	std::vector<ISelectable*> m_Selectables;
	std::vector<ISelectionListener*> m_SelectionListeners;
	bool m_AtLeastOneSelected;
};