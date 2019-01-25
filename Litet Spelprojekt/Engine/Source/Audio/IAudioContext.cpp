#include <EnginePch.h>
#include <Audio/IAudioContext.h>
#include <Audio/ALContext.h>

IAudioContext* IAudioContext::CreateContext()
{
	return new ALContext();
}