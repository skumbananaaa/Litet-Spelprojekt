#pragma once
#include "SceneInternal.h"
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Button.h>

class SceneMenu : public SceneInternal, public IButtonListener, public IExternalUIRenderer
{
public:
	SceneMenu();
	virtual ~SceneMenu();

	virtual void OnUpdate(float dtS) noexcept override;
	virtual void OnRender(float dtS) noexcept override;

	virtual void OnButtonPressed(Button* button) override;
	virtual void OnButtonReleased(Button* button) override;
	virtual void OnButtonHovered(Button* button) override;
	virtual void OnButtonNotHovered(Button* button) override;

	virtual void OnRenderGUIObject(GUIContext* context, GUIObject* object) override;

private:
	Panel* m_pPanel;
	Button* m_pButtonPlay;
	Button* m_pButtonOptions;
	Button* m_pButtonCredits;
	Button* m_pButtonQuit;
};
