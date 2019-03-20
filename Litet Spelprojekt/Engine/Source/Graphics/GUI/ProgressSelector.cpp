#include <EnginePch.h>
#include "..\..\..\Include\Graphics\GUI\ProgressSelector.h"

ProgressSelector::ProgressSelector(float x, float y, float width, float height) : ProgressBar(x, y, width, height), m_IsPressed(false)
{
}

ProgressSelector::~ProgressSelector()
{
}

void ProgressSelector::OnAdded(GUIObject* parent)
{
	AddMouseListener(this);
}

void ProgressSelector::OnRemoved(GUIObject* parent)
{
	RemoveRealTimeRenderer();
	RemoveMouseListener(this);
}

void ProgressSelector::OnMouseMove(const glm::vec2& position)
{
	if (m_IsPressed)
	{
		SetPercentage((position.x - GetXInWorld()) / GetWidth());
	}
	else
	{
		/*if (ContainsPoint(position))
		{
			if (!m_IsHovered)
			{
				m_IsHovered = true;
				RequestRepaint();
			}
		}
		else if (m_IsHovered)
		{
			m_IsHovered = false;
			RequestRepaint();
		}*/
	}
}

void ProgressSelector::OnMousePressed(const glm::vec2& position, MouseButton mousebutton)
{
	if (ContainsPoint(position))
	{
		AddRealTimeRenderer();
		m_IsPressed = true;
		SetPercentage((position.x - GetXInWorld()) / GetWidth());
	}
}

void ProgressSelector::OnMouseReleased(const glm::vec2& position, MouseButton mousebutton)
{
	if (m_IsPressed)
	{
		RemoveRealTimeRenderer();
		RequestRepaint();
		m_IsPressed = false;
	}
}

void ProgressSelector::PrintName() const
{
	std::cout << "ProgressSelector";
}