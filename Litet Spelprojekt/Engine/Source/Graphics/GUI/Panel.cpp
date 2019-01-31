#include <EnginePch.h>
#include <Graphics/GUI/Panel.h>

Panel::Panel(float x, float y, float width, float height) : GUIObject(x, y, width, height)
{
	SetBackgroundColor(glm::vec4(0.118F, 0.118F, 0.118F, 1.0F));
}

Panel::~Panel()
{

}

void Panel::PrintName() const
{
	std::cout << "Panel";
}
