#include <EnginePch.h>
#include <IO/SOUND.h>
#include <IO/ResourceHandler.h>

uint32 SOUND::MONO_FART = 0;

void SOUND::RegisterResources()
{
	MONO_FART					= ResourceHandler::RegisterSound("Mono/fart.wav");
}