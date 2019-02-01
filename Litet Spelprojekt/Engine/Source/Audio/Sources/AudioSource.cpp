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
	alSourcei(m_SourceId, AL_LOOPING, FALSE);
}

AudioSource::AudioSource(const Music& music) noexcept
{
	alGenSources(1, &m_SourceId);
	alSourcei(m_SourceId, AL_BUFFER, music.m_BufferId);
	alSourcef(m_SourceId, AL_GAIN, 1.0f);
	alSourcef(m_SourceId, AL_PITCH, 1.0f);
	alSourcei(m_SourceId, AL_LOOPING, FALSE);
	alSourcef(m_SourceId, AL_ROLLOFF_FACTOR, 0.0f);
}

AudioSource::~AudioSource()
{
	alDeleteSources(1, &m_SourceId);
}

void AudioSource::Play() const noexcept
{
	alSourcePlay(m_SourceId);
}

void AudioSource::Pause() const noexcept
{
	alSourcePause(m_SourceId);
}

void AudioSource::TogglePause() noexcept
{
	if (m_Paused) {
		alSourcePlay(m_SourceId);
	}
	else {
		alSourcePause(m_SourceId);
	}
	m_Paused = !m_Paused;
}

void AudioSource::Stop() const noexcept
{
	alSourceStop(m_SourceId);
}

void AudioSource::SetPosition(const glm::vec3& pos) const noexcept
{
	alSource3f(m_SourceId, AL_POSITION, pos.x, pos.y, pos.z);
}

void AudioSource::SetVolume(float vol) const noexcept
{
	assert(vol >= 0.0f);
	alSourcef(m_SourceId, AL_GAIN, vol);
}

void AudioSource::SetPitch(float pitch) const noexcept
{
	assert(pitch > 0.0f);
	alSourcef(m_SourceId, AL_PITCH, pitch);
}

void AudioSource::SetLooping(bool looping) const noexcept
{
	alSourcei(m_SourceId, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
}

void AudioSource::SetRollOffFactor(float value) const noexcept
{
	alSourcef(m_SourceId, AL_ROLLOFF_FACTOR, value);
}

void AudioSource::SetReferenceDistance(float value) const noexcept
{
	alSourcef(m_SourceId, AL_REFERENCE_DISTANCE, value);
}

void AudioSource::SetMaxDistance(float value) const noexcept
{
	alSourcef(m_SourceId, AL_MAX_DISTANCE, value);
}

