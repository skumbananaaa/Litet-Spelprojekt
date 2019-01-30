#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>

class API Panel : public GUIObject
{
public:
	Panel(float x, float y, float width, float height, Texture2D* texture);
	virtual ~Panel();

protected:
	virtual void PrintName() const override;
};