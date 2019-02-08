#include <EnginePch.h>
#include <IO/MATERIAL.h>
#include <IO/ResourceHandler.h>

uint32 MATERIAL::BLACK = 0;
uint32 MATERIAL::WHITE = 0;
uint32 MATERIAL::RED = 0;
uint32 MATERIAL::GREEN = 0;
uint32 MATERIAL::BLUE = 0;
uint32 MATERIAL::BOAT = 0;
uint32 MATERIAL::GROUND = 0;
uint32 MATERIAL::SINGLE_BED = 0;
uint32 MATERIAL::BUNK_BED = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void MATERIAL::RegisterResourcesPreLoading()
{

}

void MATERIAL::RegisterResources()
{
	BLACK				= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 0.0F, 0.0F, 1.0F));
	WHITE				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 1.0F, 1.0F, 1.0F));
	RED					= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	GREEN				= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	BLUE				= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	BOAT				= ResourceHandler::RegisterMaterial(TEXTURE::SHIP, TEXTURE::SHIP_NORMAL);
	GROUND				= ResourceHandler::RegisterMaterial(glm::vec4(0.471f, 0.282f, 0.11f, 1.0f));
	SINGLE_BED			= ResourceHandler::RegisterMaterial(TEXTURE::SINGLE_BED);
	BUNK_BED			= ResourceHandler::RegisterMaterial(TEXTURE::BUNK_BED);
}