#pragma once
#include "SceneInternal.h"
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Button.h>
#include <Graphics/GUI/ListScrollable.h>
#include <Graphics/GUI/SelectionHandler.h>
#include <World/Scenarios/IScenario.h>

class AudioSource;

class SceneScenario : public SceneInternal, public ISelectionListener, public IButtonListener, public IExternalUIRenderer
{
	friend class Game;

public:
	virtual ~SceneScenario();

	virtual void OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept override;
	virtual void OnDeactivated(SceneInternal* newScene) noexcept override;

	virtual void OnSelected(const SelectionHandler* handler, ISelectable* selection) override;
	virtual void OnDeselected(const SelectionHandler* handler, ISelectable* selection) override;

	virtual void OnButtonPressed(Button* button) override;
	virtual void OnButtonReleased(Button* button) override;
	virtual void OnButtonHovered(Button* button) override;
	virtual void OnButtonNotHovered(Button* button) override;

	virtual void OnRenderGUIObject(GUIContext* context, GUIObject* object) override;

protected:
	SceneScenario();

private:
	Panel* m_pPanel;
	TextView* m_pTextViewTitle;
	Button* m_pButtonBack;
	Button* m_pButtonStart;
	ListScrollable* m_pListScrollable;
	SelectionHandler m_SelectionHandler;
	AudioSource* m_pAudioSourceSelect;
	AudioSource* m_pAudioSourceDeselect;
	AudioSource* m_pAudioSourceHover;
};
