#include <EnginePch.h>
#include <Audio/Listeners/AudioListener.h>

void AudioListener::SetPosition(const glm::vec3& pos) noexcept
{
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
}
