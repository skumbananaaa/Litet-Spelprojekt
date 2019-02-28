#pragma once
#include "SceneInternal.h"
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Button.h>

class SceneOptions : public SceneInternal, public IButtonListener
{
	friend class Game;

public:
	virtual ~SceneOptions();

	virtual void OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept override;

	virtual void OnUpdate(float dtS) noexcept override;
	virtual void OnRender(float dtS) noexcept override;

	virtual void OnButtonPressed(Button* button) override;
	virtual void OnButtonReleased(Button* button) override;
	virtual void OnButtonHovered(Button* button) override;
	virtual void OnButtonNotHovered(Button* button) override;

protected:
	SceneOptions();

private:
	Panel* m_pPanel;
	Button* m_pButtonBack;
	TextView* m_pTextViewTitle;
};
