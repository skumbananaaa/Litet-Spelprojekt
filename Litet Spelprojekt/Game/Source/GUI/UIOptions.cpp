#include "..\..\Include\GUI\UIOptions.h"
#include <World/Settings.h>

UIOptions::UIOptions(float x, float y, float width, float height) : Panel(x, y, width, height)
{
	float xIndent = 10;
	float xOffset = 200;
	float progressHeight = 35;
	float yOffset = progressHeight + 15;

	m_CameraRotateSensitivity	= new ProgressSelector(xOffset, height - yOffset * 2, width - xIndent - xOffset, progressHeight);
	m_CameraMoveXSensitivity	= new ProgressSelector(xOffset, height - yOffset * 3, width - xIndent - xOffset, progressHeight);
	m_CameraMoveYSensitivity	= new ProgressSelector(xOffset, height - yOffset * 4, width - xIndent - xOffset, progressHeight);
	m_CameraZoomSensitivity		= new ProgressSelector(xOffset, height - yOffset * 5, width - xIndent - xOffset, progressHeight);

	m_VolumeMaster				= new ProgressSelector(xOffset, height - yOffset * 8, width - xIndent - xOffset, progressHeight);
	m_VolumeMusic				= new ProgressSelector(xOffset, height - yOffset * 9, width - xIndent - xOffset, progressHeight);
	m_VolumeSoundEffect			= new ProgressSelector(xOffset, height - yOffset * 10, width - xIndent - xOffset, progressHeight);

	m_CameraRotateSensitivity->SetPercentage(Settings::GetCameraRotateSensitivity() * 25.0F);
	m_CameraMoveXSensitivity->SetPercentage(Settings::GetCameraMoveXSensitivity());
	m_CameraMoveYSensitivity->SetPercentage(Settings::GetCameraMoveYSensitivity());
	m_CameraZoomSensitivity->SetPercentage(Settings::GetCameraZoomSensitivity());
	m_VolumeMaster->SetPercentage(Settings::GetVolumeMaster());
	m_VolumeMusic->SetPercentage(Settings::GetVolumeMusic());
	m_VolumeSoundEffect->SetPercentage(Settings::GetVolumeSoundEffect());

	m_CameraRotateSensitivity->AddProgressListener(this);
	m_CameraMoveXSensitivity->AddProgressListener(this);
	m_CameraMoveYSensitivity->AddProgressListener(this);
	m_CameraZoomSensitivity->AddProgressListener(this);
	m_VolumeMaster->AddProgressListener(this);
	m_VolumeMusic->AddProgressListener(this);
	m_VolumeSoundEffect->AddProgressListener(this);

	Add(m_CameraRotateSensitivity);
	Add(m_CameraMoveXSensitivity);
	Add(m_CameraMoveYSensitivity);
	Add(m_CameraZoomSensitivity);
	Add(m_VolumeMaster);
	Add(m_VolumeMusic);
	Add(m_VolumeSoundEffect);


	float xOffsetText = 10;

	Add(new TextView(xOffsetText, height - yOffset * 2, xOffset - xOffsetText, progressHeight, "Känslighet Rotation"));
	Add(new TextView(xOffsetText, height - yOffset * 3, xOffset - xOffsetText, progressHeight, "Känslighet X-led"));
	Add(new TextView(xOffsetText, height - yOffset * 4, xOffset - xOffsetText, progressHeight, "Känslighet Y-led"));
	Add(new TextView(xOffsetText, height - yOffset * 5, xOffset - xOffsetText, progressHeight, "Känslighet Zoom"));

	Add(new TextView(xOffsetText, height - yOffset * 8, xOffset - xOffsetText, progressHeight, "Volym Master"));
	Add(new TextView(xOffsetText, height - yOffset * 9, xOffset - xOffsetText, progressHeight, "Volym Musik"));
	Add(new TextView(xOffsetText, height - yOffset * 10, xOffset - xOffsetText, progressHeight, "Volym Effekter"));

	Add(new TextView(xOffsetText, height - yOffset * 1, width, progressHeight, "Kamera", false, 70));
	Add(new TextView(xOffsetText, height - yOffset * 7, width, progressHeight, "Ljud", false, 70));
}

UIOptions::~UIOptions()
{
}

void UIOptions::OnProgressChange(ProgressBar* pProgressBar, float percentage)
{
	if (pProgressBar == m_CameraRotateSensitivity)
	{
		Settings::SetCameraRotateSensitivity(percentage / 25.0F);
	}
	else if(pProgressBar == m_CameraMoveXSensitivity)
	{
		Settings::SetCameraMoveXSensitivity(percentage);
	}
	else if (pProgressBar == m_CameraMoveYSensitivity)
	{
		Settings::SetCameraMoveYSensitivity(percentage);
	}
	else if (pProgressBar == m_CameraZoomSensitivity)
	{
		Settings::SetCameraZoomSensitivity(percentage);
	}
	else if (pProgressBar == m_VolumeMaster)
	{
		Settings::SetVolumeMaster(percentage);
	}
	else if (pProgressBar == m_VolumeMusic)
	{
		Settings::SetVolumeMusic(percentage);
	}
	else if (pProgressBar == m_VolumeSoundEffect)
	{
		Settings::SetVolumeSoundEffect(percentage);
	}
}

void UIOptions::PrintName() const
{
	std::cout << "UIOptions";
}