#include <EnginePch.h>
#include <IO/SOUND.h>
#include <IO/ResourceHandler.h>

uint32 SOUND::MONO_FART = 0;
uint32 SOUND::MONO_FIREALARM = 0;
uint32 SOUND::UI_SELECT = 0;
uint32 SOUND::UI_DESELECT = 0;
uint32 SOUND::UI_HOVER = 0;
uint32 SOUND::MONO_EXPLOSION = 0;
uint32 SOUND::MONO_SCREAM = 0;
uint32 SOUND::MONO_FIRE = 0;
uint32 SOUND::MONO_CRASH = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void SOUND::RegisterResourcesPreLoading()
{

}

void SOUND::RegisterResources()
{
	MONO_FART					= ResourceHandler::RegisterSound("Mono/fart.wav");
	MONO_FIREALARM				= ResourceHandler::RegisterSound("Mono/FireAlarm.wav");
	UI_SELECT					= ResourceHandler::RegisterSound("Mono/Select.wav");
	UI_DESELECT					= ResourceHandler::RegisterSound("Mono/Deselect.wav");
	UI_HOVER					= ResourceHandler::RegisterSound("Mono/Hover.wav");
	MONO_EXPLOSION				= ResourceHandler::RegisterSound("Mono/Explosion.wav");
	MONO_SCREAM					= ResourceHandler::RegisterSound("Mono/WilhelmScream.wav");
	MONO_FIRE					= ResourceHandler::RegisterSound("Mono/Fire.wav");
	MONO_CRASH					= ResourceHandler::RegisterSound("Mono/Crash.wav");
}