#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/PanelScrollable.h>

class API ListScrollable : public PanelScrollable
{
public:
	ListScrollable(float x, float y, float width, float height);
	virtual ~ListScrollable();

	virtual void Add(GUIObject* child) noexcept override;

protected:
	int32 GetHeightOfChildren() noexcept;

	virtual void PrintName() const override;

private:
};