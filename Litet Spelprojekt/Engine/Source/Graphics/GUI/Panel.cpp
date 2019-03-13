#include <EnginePch.h>
#include <Graphics/GUI/Panel.h>

Panel::Panel(float x, float y, float width, float height) : GUIObject(x, y, width, height)
{
	SetBackgroundColor(GUIContext::COLOR_PANEL_BACKGROUND);
}

Panel::~Panel()
{

}

void Panel::OnMouseMove(const glm::vec2& position)
{
	if (ContainsPoint(position))
	{
		if (!m_IsHovered)
		{
			m_IsHovered = true;
			OnHovered(position);
			RequestRepaint();
		}
	}
	else if (m_IsHovered)
	{
		m_IsHovered = false;
		OnNotHovered(position);
		RequestRepaint();
	}
	TriggerSendUpdate(this);
}

void Panel::SetHovered(bool hovered)
{
	if (IsHovered() != hovered)
	{
		IHoverable::SetHovered(hovered);
		RequestRepaint();
	}
}

void Panel::OnAdded(GUIObject* parent)
{
	AddMouseListener(this);
}

void Panel::OnRemoved(GUIObject* parent)
{
	RemoveMouseListener(this);
}

void Panel::PrintName() const
{
	std::cout << "Panel";
}
