#include <EnginePch.h>
#include <Audio/Listeners/AudioListener.h>

void AudioListener::SetPosition(const glm::vec3& pos) noexcept
{
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}

void AudioListener::SetOrientation(const glm::vec3& lookAt, const glm::vec3& up) noexcept
{
	float lookAt_Up[6];
	lookAt_Up[0] = lookAt.x; 
	lookAt_Up[1] = lookAt.y;
	lookAt_Up[2] = lookAt.z;
	lookAt_Up[3] = up.x;
	lookAt_Up[4] = up.y;
	lookAt_Up[5] = up.z;

	alListenerfv(AL_ORIENTATION, lookAt_Up);
}

void AudioListener::SetMasterVolume(float vol) noexcept
{
	assert(vol >= 0.0f);
	alListenerf(AL_GAIN, vol);
}
