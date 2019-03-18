#include <EnginePch.h>
#include <Graphics\GUI\SelectionHandler.h>

SelectionHandler::SelectionHandler(bool atLeastOne, bool multipleSelections)
{
	m_AtLeastOneSelected = atLeastOne;
	m_MultipleSelections = multipleSelections;
}

SelectionHandler::~SelectionHandler()
{
	
}

void SelectionHandler::Release()
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

void SelectionHandler::OnSelected(ISelectable* selected)
{
	std::vector<ISelectable*> selection;
	GetSelection(selection);

	if (!Contains<ISelectable>(selection, selected))  //We Clicked on a non selected Object
	{
		if (!m_MultipleSelections && !selection.empty())
		{
			selection[0]->SetSelected(false);
			TriggerOnDeselected(selection[0]);
		}
		if (selected != nullptr)
		{
			selected->SetSelected(true);
			TriggerOnSelected(selected);
		}
	}
	else //We Clicked on an already selected Object
	{
		if ((!selection.empty() && !m_AtLeastOneSelected) || (!(selection.size() > 1) && m_MultipleSelections))
		{
			selected->SetSelected(false);
			TriggerOnDeselected(selected);
		}
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

void SelectionHandler::GetSelection(std::vector<ISelectable*>& selection)
{
	for (ISelectable* object : m_Selectables)
	{
		if (object->IsSelected())
		{
			selection.push_back(object);
		}
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
}
