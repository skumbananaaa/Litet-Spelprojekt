#include <EnginePch.h>
#include <Graphics/GUI/Panel.h>

Panel::Panel(float x, float y, float width, float height, Texture2D* texture) : GUIObject(x, y, width, height)
{
	SetTexture(texture);
}

Panel::~Panel()
{

}