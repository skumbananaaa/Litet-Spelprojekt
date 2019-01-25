#include <EnginePch.h>
#include <Audio/Sources/AudioSource.h>

AudioSource::AudioSource(const SoundEffect& soundEffect, const glm::vec3& pos, const glm::vec3& vel) noexcept
{
	alGenSources(1, &m_SourceId);
	alSourcei(m_SourceId, AL_BUFFER, soundEffect.m_BufferId);
	alSource3f(m_SourceId, AL_POSITION, pos.x, pos.y, pos.z);
	alSource3f(m_SourceId, AL_VELOCITY, vel.x, vel.y, vel.z);
	alSourcef(m_SourceId, AL_GAIN, 1.0f);
	alSourcef(m_SourceId, AL_PITCH, 1.0f);
	alSourcei(m_SourceId, AL_LOOPING, true);
}

AudioSource::~AudioSource()
{
	alDeleteSources(1, &m_SourceId);
}

void AudioSource::Play() noexcept
{
	alSourcePlay(m_SourceId);
}

void AudioSource::Pause() noexcept
{
	alSourcePause(m_SourceId);
}

void AudioSource::Stop() noexcept
{
	alSourceStop(m_SourceId);
}

void AudioSource::SetPosition(const glm::vec3& pos) noexcept
{
	alSource3f(m_SourceId, AL_POSITION, pos.x, pos.y, pos.z);
}

void AudioSource::SetVelocity(const glm::vec3& vel) noexcept
{
	alSource3f(m_SourceId, AL_VELOCITY, vel.x, vel.y, vel.z);
}

void AudioSource::SetVolume(float vol) noexcept
{
	alSourcef(m_SourceId, AL_GAIN, vol);
}

void AudioSource::SetPitch(float pitch) noexcept
{
	alSourcef(m_SourceId, AL_PITCH, pitch);
}

void AudioSource::SetLooping(bool looping) noexcept
{
	alSourcei(m_SourceId, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}
