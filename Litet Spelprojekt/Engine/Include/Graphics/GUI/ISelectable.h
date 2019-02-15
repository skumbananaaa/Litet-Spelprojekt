#pragma once
#include <EnginePch.h>

class ISelectable;

class API ISelectableListener
{
public:
	virtual void OnSelected(ISelectable* selection) = 0;
	virtual void OnDeselected(ISelectable* selection) = 0;
};

class API ISelectable
{
public:
	virtual void SetSelected(bool selected)
	{
		m_IsSelected = selected;
	};

	virtual bool IsSelected() const
	{
		return m_IsSelected;
	};

	virtual bool AddSelectionListener(ISelectableListener* listener)
	{
		for (ISelectableListener* object : m_SelectionListeners)
		{
			if (object == listener)
			{
				std::cout << "ISelectionListener already added!" << std::endl;
				return false;
			}
		}
		m_SelectionListeners.push_back(listener);
		return true;
	};

	virtual void RemoveSelectionListener(ISelectableListener* listener)
	{
		int32 counter = 0;
		for (ISelectableListener* object : m_SelectionListeners)
		{
			if (object == listener)
			{
				m_SelectionListeners.erase(m_SelectionListeners.begin() + counter);
				return;
			}
			counter++;
		}
	};

protected:
	ISelectable() : m_IsSelected(false) {};

	virtual const std::vector<ISelectableListener*> GetSelectionListeners() const
	{
		return m_SelectionListeners;
	};

	virtual void TriggerOnSelected(ISelectable* selection) const
	{
		for (ISelectableListener* listener : m_SelectionListeners)
		{
			listener->OnSelected(selection);
		}
	};

	virtual void TriggerOnDeselected(ISelectable* selection) const
	{
		for (ISelectableListener* listener : m_SelectionListeners)
		{
			listener->OnDeselected(selection);
		}
	};

	std::vector<ISelectableListener*> m_SelectionListeners;
	bool m_IsSelected;
};