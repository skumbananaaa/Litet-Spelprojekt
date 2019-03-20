#include <EnginePch.h>
#include <Audio/Sources/AudioSource.h>
#include <World/Settings.h>

std::vector<AudioSource*> AudioSource::s_References;

AudioSource::AudioSource(const SoundEffect& soundEffect, const glm::vec3& pos, const glm::vec3& vel) noexcept
{
	alGenSources(1, &m_SourceId);
	alSourcei(m_SourceId, AL_BUFFER, soundEffect.m_BufferId);
	alSource3f(m_SourceId, AL_POSITION, pos.x, pos.y, pos.z);
	alSource3f(m_SourceId, AL_VELOCITY, vel.x, vel.y, vel.z);
	alSourcef(m_SourceId, AL_GAIN, 1.0f);
	alSourcef(m_SourceId, AL_PITCH, 1.0f);
	alSourcei(m_SourceId, AL_LOOPING, FALSE);
	m_IsMusic = false;
	s_References.push_back(this);
}

AudioSource::AudioSource(const Music& music) noexcept
{
	alGenSources(1, &m_SourceId);
	alSourcei(m_SourceId, AL_BUFFER, music.m_BufferId);
	alSourcef(m_SourceId, AL_GAIN, 1.0f);
	alSourcef(m_SourceId, AL_PITCH, 1.0f);
	alSourcei(m_SourceId, AL_LOOPING, FALSE);
	alSourcef(m_SourceId, AL_ROLLOFF_FACTOR, 0.0f);
	m_IsMusic = true;
	s_References.push_back(this);
}

AudioSource::~AudioSource()
{
	for (int i = 0; i < s_References.size(); i++)
	{
		if (s_References[i] == this)
		{
			s_References.erase(s_References.begin() + i);
			break;
		}
	}
	alDeleteSources(1, &m_SourceId);
}

void AudioSource::Play() const noexcept
{
	if (!IsPlaying())
	{
		if (m_IsMusic)
		{
			SetVolume(Settings::GetVolumeMusic());
		}
		else
		{
			SetVolume(Settings::GetVolumeSoundEffect());
		}

		alSourcePlay(m_SourceId);
	}
}

void AudioSource::Pause() const noexcept
{
	alSourcePause(m_SourceId);
}

void AudioSource::TogglePause() noexcept
{
	if (IsPlaying())
	{
		Pause();
	}
	else
	{
		Play();
	}
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

bool AudioSource::IsPlaying() const noexcept
{
	ALint result;
	alGetSourcei(m_SourceId, AL_SOURCE_STATE, &result);
	return result == AL_PLAYING;
}

AudioSource* AudioSource::CreateSoundSource(int32 sound, const glm::vec3& pos, const glm::vec3& vel)
{
	const SoundEffect* pSound = ResourceHandler::GetSound(sound);
	if (pSound)
	{
		return new AudioSource(*pSound, pos, vel);
	}
	return nullptr;
}

AudioSource* AudioSource::CreateMusicSource(int32 music)
{
	const Music* pMusic = ResourceHandler::GetMusic(music);
	if (pMusic)
	{
		return new AudioSource(*pMusic);
	}
	return nullptr;
}

void AudioSource::SetMusicVolume(float volume) noexcept
{
	for (int i = 0; i < s_References.size(); i++)
	{
		if (s_References[i]->m_IsMusic)
		{
			s_References[i]->SetVolume(volume);
		}
	}
}

void AudioSource::SetSoundVolume(float volume) noexcept
{
	for (int i = 0; i < s_References.size(); i++)
	{
		if (!s_References[i]->m_IsMusic)
		{
			s_References[i]->SetVolume(volume);
		}
	}
}