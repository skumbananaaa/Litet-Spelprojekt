#pragma once
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/ProgressSelector.h>

class UIOptions : public Panel, public IProgressListener
{
public:
	UIOptions(float x, float y, float width, float height);
	virtual ~UIOptions();

	virtual void OnProgressChange(ProgressBar* pProgressBar, float percentage) override;

protected:

	virtual void PrintName() const override;

private:
	ProgressSelector* m_CameraRotateSensitivity;
	ProgressSelector* m_CameraMoveXSensitivity;
	ProgressSelector* m_CameraMoveYSensitivity;
	ProgressSelector* m_CameraZoomSensitivity;

	ProgressSelector* m_VolumeMaster;
	ProgressSelector* m_VolumeMusic;
	ProgressSelector* m_VolumeSoundEffect;
};
