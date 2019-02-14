#pragma once
#include <EnginePch.h>
#include <Graphics/GameObject.h>
#include <Audio/Sources/AudioSource.h>

class API VisualAudioSource : public GameObject
{
public:

	static VisualAudioSource* CreateObject(int32 sound, int32 mesh, int32 mat, float volume = 1.0f, float attenuation = 1.0f);
private:
	AudioSource* m_pAudio;
	VisualAudioSource(int32 audioSrc, int32 mesh, int32 material);
};