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

	SetClientSize(GetClientWidth(), GetHeightOfChildren());

	int32 y = GetClientHeight();

	for (int i = GetChildrenToAdd().size() - 1; i >= 0; i--)
	{
		y -= GetChildrenToAdd()[i]->GetHeight();
		GetChildrenToAdd()[i]->SetPosition(0, y);
	}

	for (int i = GetChildren().size() - 1; i >= 0; i--)
	{
		y -= GetChildren()[i]->GetHeight();
		GetChildren()[i]->SetPosition(0, y);
	}
}

int32 ListScrollable::GetHeightOfChildren() noexcept
{
	int height = 0;
	for (GUIObject* object : GetChildren())
	{
		height += object->GetHeight();
	}
	for (GUIObject* object : GetChildrenToAdd())
	{
		height += object->GetHeight();
	}
	for (GUIObject* object : GetChildrenToRemove())
	{
		height -= object->GetHeight();
	}
	return height;
}

void ListScrollable::PrintName() const
{
	std::cout << "ListScrollable" << std::endl;
}
