#include <EnginePch.h>
#include <Graphics\GUI\ListScrollable.h>

ListScrollable::ListScrollable(float x, float y, float width, float height) : PanelScrollable(x, y, width, height, width, height)
{

}

ListScrollable::~ListScrollable()
{

}

void ListScrollable::Add(GUIObject* child) noexcept
{
	PanelScrollable::Add(child);

	SetClientSize((float)GetClientWidth(), (float)GetHeightOfChildren());

	int32 y = (int32)GetClientHeight();
	for (int i = (int32)GetChildrenToAdd().size() - 1; i >= 0; i--)
	{
		y -= (int32)GetChildrenToAdd()[i]->GetActualHeight();
		GetChildrenToAdd()[i]->SetPosition(GetChildrenToAdd()[i]->GetMargin().x, (float)y);
	}

	for (int i = (int32)GetChildren().size() - 1; i >= 0; i--)
	{
		y -= (int32)GetChildren()[i]->GetActualHeight();
		GetChildren()[i]->SetPosition(GetChildren()[i]->GetMargin().x, (float)y);
	}
}

int32 ListScrollable::GetHeightOfChildren() noexcept
{
	int height = 0;
	for (GUIObject* object : GetChildren())
	{
		height += (int32)object->GetActualHeight();
	}
	for (GUIObject* object : GetChildrenToAdd())
	{
		height += (int32)object->GetActualHeight();
	}
	return height;
}

void ListScrollable::PrintName() const
{
	std::cout << "ListScrollable" << std::endl;
}
