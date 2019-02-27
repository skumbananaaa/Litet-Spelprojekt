#pragma once
#include "SceneInternal.h"
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Button.h>

class SceneOptions : public SceneInternal
{
public:
	SceneOptions();
	virtual ~SceneOptions();

	virtual void OnUpdate(float dtS) noexcept override;
	virtual void OnRender(float dtS) noexcept override;



private:
	Panel* m_pPanel;
	Button* m_pButtonPlay;
	Button* m_pButtonCredits;
	Button* m_pButtonQuit;
};
