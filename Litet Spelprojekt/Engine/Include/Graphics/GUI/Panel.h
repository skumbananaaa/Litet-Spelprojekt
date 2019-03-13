#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>
#include <Graphics/GUI/IHoverable.h>

class API Panel : public GUIObject, public IHoverable
{
public:
	Panel(float x, float y, float width, float height);
	virtual ~Panel();

	virtual void OnMouseMove(const glm::vec2& position) override;
	virtual void SetHovered(bool hovered) override;

protected:
	virtual void PrintName() const override;

	virtual void OnHovered(const glm::vec2& position) {};
	virtual void OnNotHovered(const glm::vec2& position) {};

	virtual void OnAdded(GUIObject* parent) override;
	virtual void OnRemoved(GUIObject* parent) override;
};