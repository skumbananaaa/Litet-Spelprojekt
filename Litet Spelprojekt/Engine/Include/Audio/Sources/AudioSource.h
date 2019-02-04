#pragma once
#include <EnginePch.h>
#include <GLM/glm.hpp>
#include <Audio/SoundEffect.h>
#include <Audio/Music.h>

class API AudioSource
{
public:
	AudioSource(const SoundEffect& soundEffect, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& vel = glm::vec3(0.0f)) noexcept;
	AudioSource(const Music& music) noexcept;
	~AudioSource();

	void Play() const noexcept;
	void Pause() const noexcept;
	void TogglePause() noexcept;
	void Stop() const noexcept;

	void SetPosition(const glm::vec3& pos) const noexcept;
	void SetVolume(float vol) const noexcept;
	void SetPitch(float pitch) const noexcept;
	void SetLooping(bool looping) const noexcept;

	///<summary>Summary: Sets how quickly the volume decreases after the distance is larger than the reference distance. Set this and ReferenceDistance to 0 if you dont want attenuation.</summary>
	void SetRollOffFactor(float value) const noexcept;
	///<summary>Summary: Sets the distance at which the volume is 1. Set this and RollOffFactor to 0 if you dont want attenuation.</summary>
	void SetReferenceDistance(float value) const noexcept;
	///<summary>
	///<para>Summary:</para>
	///<para>Does nothing if used on non-clamped exponential or inverse model.</para>
	///<para>Sets the distance at which the volume is 0 when using the linear model.</para>
	///<para>Sets the distance at which the volume will no longer be attenuated when using clamped exponential or inverse model.</para>
	///</summary>
	void SetMaxDistance(float value) const noexcept;

private:
	uint32 m_SourceId;
	bool m_Paused = false;
};