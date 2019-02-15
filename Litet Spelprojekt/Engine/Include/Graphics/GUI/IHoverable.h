#pragma once
#include <EnginePch.h>

class IHoverable;

class API IHoverableListener
{
public:
	virtual void SendUpdate(IHoverable* selection) = 0;
};

class API IHoverable
{
public:
	virtual void SetHovered(bool hovered)
	{
		m_IsHovered = hovered;
	};

	virtual bool IsHovered() const
	{
		return m_IsHovered;
	};

	virtual bool AddHoveringListener(IHoverableListener* listener)
	{
		for (IHoverableListener* object : m_HoveringListeners)
		{
			if (object == listener)
			{
				std::cout << "IHoverableListener already added!" << std::endl;
				return false;
			}
		}
		m_HoveringListeners.push_back(listener);
		return true;
	};

	virtual void RemoveHoveringListener(IHoverableListener* listener)
	{
		int32 counter = 0;
		for (IHoverableListener* object : m_HoveringListeners)
		{
			if (object == listener)
			{
				m_HoveringListeners.erase(m_HoveringListeners.begin() + counter);
				return;
			}
			counter++;
		}
	};

protected:
	IHoverable() : m_IsHovered(false) {};

	virtual const std::vector<IHoverableListener*> GetHoveringListeners() const
	{
		return m_HoveringListeners;
	};

	virtual void TriggerSendUpdate(IHoverable* selection) const
	{
		for (IHoverableListener* listener : m_HoveringListeners)
		{
			listener->SendUpdate(selection);
		}
	};

	std::vector<IHoverableListener*> m_HoveringListeners;
	bool m_IsHovered;
};