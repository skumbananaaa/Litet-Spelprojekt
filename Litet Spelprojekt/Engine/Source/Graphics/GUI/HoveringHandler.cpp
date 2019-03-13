#include <EnginePch.h>
#include <Graphics\GUI\HoveringHandler.h>

HoveringHandler::HoveringHandler() : m_UpdateCounter(0), m_LastHoverable(nullptr)
{
	
}

HoveringHandler::~HoveringHandler()
{

}

void HoveringHandler::Release()
{
	/*for (IHoverable* object : m_Hoverables)
	{
		object->RemoveHoveringListener(this);
	}*/
	m_Hoverables.clear();
	m_HoveringListeners.clear();
}

void HoveringHandler::AddHoverable(IHoverable* selectable)
{
	if (selectable->AddHoveringListener(this))
	{
		m_Hoverables.push_back(selectable);
	}
}

void HoveringHandler::RemoveHoverable(IHoverable* selectable)
{
	selectable->RemoveHoveringListener(this);
	int32 counter = 0;
	for (IHoverable* object : m_Hoverables)
	{
		if (object == selectable)
		{
			m_Hoverables.erase(m_Hoverables.begin() + counter);
			return;
		}
		counter++;
	}
}

bool HoveringHandler::AddHoveringListener(IHoveringListener* listener)
{
	for (IHoveringListener* object : m_HoveringListeners)
	{
		if (object == listener)
		{
			std::cout << "ISelectionListener already added!" << std::endl;
			return false;
		}
	}
	m_HoveringListeners.push_back(listener);
	return true;
}

void HoveringHandler::RemoveHoveringListener(IHoveringListener* listener)
{
	int32 counter = 0;
	for (IHoveringListener* object : m_HoveringListeners)
	{
		if (object == listener)
		{
			m_HoveringListeners.erase(m_HoveringListeners.begin() + counter);
			return;
		}
		counter++;
	}
}

IHoverable* HoveringHandler::GetHovered()
{
	for (IHoverable* object : m_Hoverables)
	{
		if (object->IsHovered())
		{
			return object;
		}
	}
	return nullptr;
}

void HoveringHandler::SendUpdate(IHoverable* selection)
{
	if (++m_UpdateCounter == m_Hoverables.size())
	{
		m_UpdateCounter = 0;
		IHoverable* object = GetHovered();
		if (object != m_LastHoverable)
		{
			if (m_LastHoverable != nullptr)
			{
				TriggerOnDehovered(m_LastHoverable);
			}
			if (object != nullptr)
			{
				TriggerOnHovered(object);
			}
		}
		m_LastHoverable = object;
	}
}

void HoveringHandler::TriggerOnHovered(IHoverable* selection) const
{
	for (IHoveringListener* listener : m_HoveringListeners)
	{
		listener->OnHovered(this, selection);
	}
}

void HoveringHandler::TriggerOnDehovered(IHoverable* selection) const
{
	for (IHoveringListener* listener : m_HoveringListeners)
	{
		listener->OnDehovered(this, selection);
	}
}
