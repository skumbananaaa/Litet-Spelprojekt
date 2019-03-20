#include <EnginePch.h>
#include "..\..\Include\World\Settings.h"
#include <Audio/Sources/AudioSource.h>
#include <Audio/Listeners/AudioListener.h>

float Settings::s_CameraRotateSensitivity = 0.005f;
float Settings::s_CameraMoveXSensitivity = 0.5f;
float Settings::s_CameraMoveYSensitivity = 0.025f;
float Settings::s_CameraZoomSensitivity = 0.1f;

float Settings::s_VolumeMaster = 1.0F;
float Settings::s_VolumeMusic = 1.0F;
float Settings::s_VolumeSoundEffect = 1.0F;

void Settings::SetCameraRotateSensitivity(float sensitivity) noexcept
{
	s_CameraRotateSensitivity = glm::clamp<float>(sensitivity, 0.001F, 1.0F);
}

float Settings::GetCameraRotateSensitivity() noexcept
{
	return s_CameraRotateSensitivity;
}

void Settings::SetCameraMoveXSensitivity(float sensitivity) noexcept
{
	s_CameraMoveXSensitivity = glm::clamp<float>(sensitivity, 0.001F, 1.0F);
}

float Settings::GetCameraMoveXSensitivity() noexcept
{
	return s_CameraMoveXSensitivity;
}

void Settings::SetCameraMoveYSensitivity(float sensitivity) noexcept
{
	s_CameraMoveYSensitivity = glm::clamp<float>(sensitivity, 0.001F, 1.0F);
}

float Settings::GetCameraMoveYSensitivity() noexcept
{
	return s_CameraMoveYSensitivity;
}

void Settings::SetCameraZoomSensitivity(float sensitivity) noexcept
{
	s_CameraZoomSensitivity = glm::clamp<float>(sensitivity, 0.001F, 1.0F);
}

float Settings::GetCameraZoomSensitivity() noexcept
{
	return s_CameraZoomSensitivity;
}

void Settings::SetVolumeMaster(float volume) noexcept
{
	s_VolumeMaster = glm::clamp<float>(volume, 0.0F, 1.0F);
	AudioListener::SetMasterVolume(s_VolumeMaster);
}

float Settings::GetVolumeMaster() noexcept
{
	return s_VolumeMaster;
}

void Settings::SetVolumeMusic(float volume) noexcept
{
	s_VolumeMusic = glm::clamp<float>(volume, 0.0F, 1.0F);
	AudioSource::SetMusicVolume(s_VolumeMusic);
}

float Settings::GetVolumeMusic() noexcept
{
	return s_VolumeMusic;
}

void Settings::SetVolumeSoundEffect(float volume) noexcept
{
	s_VolumeSoundEffect = glm::clamp<float>(volume, 0.0F, 1.0F);
	AudioSource::SetSoundVolume(s_VolumeSoundEffect);
}

float Settings::GetVolumeSoundEffect() noexcept
{
	return s_VolumeSoundEffect;
}
