#include <EnginePch.h>
#include <Graphics/GUI/Panel.h>

Panel::Panel(float x, float y, float width, float height) : GUIObject(x, y, width, height)
{
	SetBackgroundColor(GUIContext::COLOR_PANEL_BACKGROUND);
}

Panel::~Panel()
{

}

void Panel::PrintName() const
{
	std::cout << "Panel";
}
