#include <EnginePch.h>
#include <IO/MUSIC.h>
#include <IO/ResourceHandler.h>

uint32 MUSIC::WAVES_AND_SEAGULLS = 0;
uint32 MUSIC::MENU = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void MUSIC::RegisterResourcesPreLoading()
{
	MENU								= ResourceHandler::RegisterMusic("AtlantisOfTheSeas.ogg");
}

void MUSIC::RegisterResources()
{
	WAVES_AND_SEAGULLS					= ResourceHandler::RegisterMusic("WavesAndSeagulls.ogg");
}


/*Atlantis Of The Sands by Serge Narcissoff | https://soundcloud.com/sergenarcissoff
Music promoted by https ://www.free-stock-music.com
Creative Commons Attribution - ShareAlike 3.0 Unported
https ://creativecommons.org/licenses/by-sa/3.0/deed.en_US*/