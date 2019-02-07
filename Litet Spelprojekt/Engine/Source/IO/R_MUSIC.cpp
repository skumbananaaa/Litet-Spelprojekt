#include <EnginePch.h>
#include <IO/MUSIC.h>
#include <IO/ResourceHandler.h>

uint32 MUSIC::WAVES_AND_SEAGULLS = 0;

void MUSIC::RegisterResources()
{
	WAVES_AND_SEAGULLS					= ResourceHandler::RegisterMusic("WavesAndSeagulls.ogg");
}