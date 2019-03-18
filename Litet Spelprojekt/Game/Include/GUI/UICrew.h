#pragma once
#include <Graphics/GUI/PanelExpandable.h>
#include "../Crew.h"
#include <Graphics/GUI/SelectionHandler.h>
#include <Graphics/GUI/HoveringHandler.h>
#include <Graphics/GUI/ProgressButton.h>

class UICrew : public IExpandableListener, public ISelectionListener, public IHoveringListener, public IExternalUIRenderer, public IButtonListener, public IProgressListener
{
public:
	UICrew(float x, float y, float width, float height, Crew* crew);
	virtual ~UICrew();

	virtual void OnExpanding(PanelExpandable* panel, float percentage) override;
	virtual void OnCollapsing(PanelExpandable* panel, float percentage) override;

	virtual void OnSelected(const SelectionHandler* handler, ISelectable* selection) override;
	virtual void OnDeselected(const SelectionHandler* handler, ISelectable* selection) override;
	virtual void OnHovered(const HoveringHandler* handler, IHoverable* selection) override;
	virtual void OnDehovered(const HoveringHandler* handler, IHoverable* selection) override;

	virtual void OnRenderGUIObject(GUIContext* context, GUIObject* object) override;

	virtual void OnButtonPressed(Button* button) override;
	virtual void OnButtonReleased(Button* button) override;
	virtual void OnButtonHovered(Button* button) override;
	virtual void OnButtonNotHovered(Button* button) override;

	virtual void OnProgressAnimationEnd(ProgressButton* progressButton) override;

	void SetVisible(bool visible) noexcept;

private:
	ProgressButton* CreateButton(const std::string& text, const glm::vec4& color, float y, float width, float height, const glm::vec2& textOffset, int shipnumber);
	PanelExpandable* CreateExpandable(const std::string& text, float x, float y, float width, float buttonHeight, uint32 icon, int index, Crew* crew, const glm::vec4& buttonColor, const glm::vec2& textOffset);

	std::vector<PanelExpandable*> m_Panels;
	SelectionHandler m_SelectionHandler;
	HoveringHandler m_HoveringHandler;
};
