#include <EnginePch.h>
#include <IO/MUSIC.h>
#include <IO/ResourceHandler.h>

uint32 MUSIC::WAVES_AND_SEAGULLS = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void MUSIC::RegisterResourcesPreLoading()
{

}

void MUSIC::RegisterResources()
{
	WAVES_AND_SEAGULLS					= ResourceHandler::RegisterMusic("WavesAndSeagulls.ogg");
}