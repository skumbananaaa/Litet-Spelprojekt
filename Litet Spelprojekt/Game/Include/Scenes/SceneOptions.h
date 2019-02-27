#pragma once
#include "SceneInternal.h"
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Button.h>

class SceneOptions : public SceneInternal
{
	friend class Game;

public:
	virtual ~SceneOptions();

	virtual void OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept override;
	virtual void OnDeactivated(SceneInternal* newScene) noexcept override;

	virtual void OnUpdate(float dtS) noexcept override;
	virtual void OnRender(float dtS) noexcept override;

protected:
	SceneOptions();

private:
	Panel* m_pPanel;
	Button* m_pButtonPlay;
	Button* m_pButtonCredits;
	Button* m_pButtonQuit;
};
