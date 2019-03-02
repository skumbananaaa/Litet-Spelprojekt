#pragma once
#include "SceneInternal.h"
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Button.h>

class AudioSource;

class SceneCredits : public SceneInternal, public IButtonListener
{
	friend class Game;

public:
	virtual ~SceneCredits();

	virtual void OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept override;

	virtual void OnUpdate(float dtS) noexcept override;
	virtual void OnRender(float dtS) noexcept override;

	virtual void OnButtonPressed(Button* button) override;
	virtual void OnButtonReleased(Button* button) override;
	virtual void OnButtonHovered(Button* button) override;
	virtual void OnButtonNotHovered(Button* button) override;

protected:
	SceneCredits();

private:
	Panel* m_pPanel;
	Button* m_pButtonBack;
	TextView* m_pTextViewTitle;
	TextView* m_pTextViewChristoffer;
	TextView* m_pTextViewHerman;
	TextView* m_pTextViewAlexander;
	TextView* m_pTextViewTim;
	TextView* m_pTextViewTimKort;
	TextView* m_pTextViewLeo;
	AudioSource* m_pAudioSourceSelect;
	AudioSource* m_pAudioSourceHover;
};
