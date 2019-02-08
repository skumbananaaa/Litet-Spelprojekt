#include <EnginePch.h>
#include <IO/DECAL.h>
#include <IO/ResourceHandler.h>

uint32 DECAL::BLOOD = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void DECAL::RegisterResourcesPreLoading()
{

}

void DECAL::RegisterResources()
{
	BLOOD					= ResourceHandler::RegisterDecal(TEXTURE::BLOOD, TEXTURE::BLOOD_NORMAL);
}