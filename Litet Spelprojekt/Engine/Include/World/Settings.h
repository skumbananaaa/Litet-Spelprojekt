#pragma once
#include <EnginePch.h>

class API Settings
{
public:
	static void SetCameraRotateSensitivity(float sensitivity) noexcept;
	static float GetCameraRotateSensitivity() noexcept;

	static void SetCameraMoveXSensitivity(float sensitivity) noexcept;
	static float GetCameraMoveXSensitivity() noexcept;

	static void SetCameraMoveYSensitivity(float sensitivity) noexcept;
	static float GetCameraMoveYSensitivity() noexcept;

	static void SetCameraZoomSensitivity(float sensitivity) noexcept;
	static float GetCameraZoomSensitivity() noexcept;


	static void SetVolumeMaster(float volume) noexcept;
	static float GetVolumeMaster() noexcept;

	static void SetVolumeMusic(float volume) noexcept;
	static float GetVolumeMusic() noexcept;

	static void SetVolumeSoundEffect(float volume) noexcept;
	static float GetVolumeSoundEffect() noexcept;

private:
	static float s_CameraRotateSensitivity;
	static float s_CameraMoveXSensitivity;
	static float s_CameraMoveYSensitivity;
	static float s_CameraZoomSensitivity;

	static float s_VolumeMaster;
	static float s_VolumeMusic;
	static float s_VolumeSoundEffect;
};