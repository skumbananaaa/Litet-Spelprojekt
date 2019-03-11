#pragma once
#include "SceneInternal.h"
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Button.h>
#include <Graphics/Textures/Texture2D.h>

class AudioSource;
class SceneGame;

class SceneInstructions : public SceneInternal, public IButtonListener, public IExternalUIRenderer
{
	friend class Game;

public:
	virtual ~SceneInstructions();

	virtual void OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept override;

	virtual void OnUpdate(float dtS) noexcept override;
	virtual void OnRender(float dtS) noexcept override;

	virtual void OnButtonPressed(Button* button) override;
	virtual void OnButtonReleased(Button* button) override;
	virtual void OnButtonHovered(Button* button) override;
	virtual void OnButtonNotHovered(Button* button) override;

	virtual void OnRenderGUIObject(GUIContext* context, GUIObject* object) override;

	void CreateWorld() noexcept;

protected:
	SceneInstructions();

private:
	SceneGame* m_pSceneGame;
	Panel* m_pPanel;
	Button* m_pButtonContinue;
	bool m_HasRenderedUI;
};
