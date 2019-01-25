#pragma once
#include <EnginePch.h>
#include <GLM/glm.hpp>
#include <Audio/SoundEffect.h>

class API AudioSource
{
public:
	AudioSource(const SoundEffect& soundEffect, const glm::vec3& pos = glm::vec3(0.0f), const glm::vec3& vel = glm::vec3(0.0f)) noexcept;
	~AudioSource();

	void Play() noexcept;
	void Pause() noexcept;
	void Stop() noexcept;

	void SetPosition(const glm::vec3& pos) noexcept;
	void SetVelocity(const glm::vec3& vel) noexcept;
	void SetVolume(float vol) noexcept;
	void SetPitch(float pitch) noexcept;
	void SetLooping(bool looping) noexcept;

private:
	uint32 m_SourceId;

};