#include <EnginePch.h>
#include <Graphics\GUI\SelectionHandler.h>

SelectionHandler::SelectionHandler(bool atLeastOne)
{
	m_AtLeastOneSelected = atLeastOne;
}

SelectionHandler::~SelectionHandler()
{
	for (ISelectable* object : m_Selectables)
	{
		object->RemoveSelectionListener(this);
	}
	m_Selectables.clear();
	m_SelectionListeners.clear();
}

void SelectionHandler::AddSelectable(ISelectable* selectable)
{
	if (selectable->AddSelectionListener(this))
	{
		m_Selectables.push_back(selectable);
	}
	if (m_Selectables.size() == 1 && m_AtLeastOneSelected)
	{
		OnSelected(selectable);
	}
}

void SelectionHandler::RemoveSelectable(ISelectable* selectable)
{
	selectable->RemoveSelectionListener(this);
	int32 counter = 0;
	for (ISelectable* object : m_Selectables)
	{
		if (object == selectable)
		{
			m_Selectables.erase(m_Selectables.begin() + counter);
			return;
		}
		counter++;
	}
}

void SelectionHandler::OnSelected(ISelectable* selection)
{
	ISelectable* lastObject = nullptr;

	for (ISelectable* object : m_Selectables)
	{
		if (object->IsSelected())
		{
			lastObject = object;
			break;
		}
	}
	if (lastObject != selection)
	{
		if (lastObject != nullptr)
		{
			lastObject->SetSelected(false);
			TriggerOnDeselected(lastObject);
		}
		selection->SetSelected(true);
		TriggerOnSelected(selection);
	}
	else if (!m_AtLeastOneSelected && lastObject != nullptr)
	{
		lastObject->SetSelected(false);
		TriggerOnDeselected(lastObject);
	}
}

bool SelectionHandler::AddSelectionListener(ISelectionListener* listener)
{
	for (ISelectionListener* object : m_SelectionListeners)
	{
		if (object == listener)
		{
			std::cout << "ISelectionListener already added!" << std::endl;
			return false;
		}
	}
	m_SelectionListeners.push_back(listener);
	return true;
}

void SelectionHandler::RemoveSelectionListener(ISelectionListener* listener)
{
	int32 counter = 0;
	for (ISelectionListener* object : m_SelectionListeners)
	{
		if (object == listener)
		{
			m_SelectionListeners.erase(m_SelectionListeners.begin() + counter);
			return;
		}
		counter++;
	}
}

void SelectionHandler::TriggerOnSelected(ISelectable* selection) const
{
	for (ISelectionListener* listener : m_SelectionListeners)
	{
		listener->OnSelected(this, selection);
	}
}
void SelectionHandler::TriggerOnDeselected(ISelectable* selection) const
{
	for (ISelectionListener* listener : m_SelectionListeners)
	{
		listener->OnDeselected(this, selection);
	}
};