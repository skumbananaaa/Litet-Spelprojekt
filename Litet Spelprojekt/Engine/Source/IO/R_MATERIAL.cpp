#include <EnginePch.h>
#include <IO/MATERIAL.h>
#include <IO/ResourceHandler.h>

uint32 MATERIAL::BLACK = 0;
uint32 MATERIAL::WHITE = 0;
uint32 MATERIAL::RED = 0;
uint32 MATERIAL::RED_1 = 0;
uint32 MATERIAL::RED_2 = 0;
uint32 MATERIAL::RED_3 = 0;
uint32 MATERIAL::RED_4 = 0;
uint32 MATERIAL::GREEN = 0;
uint32 MATERIAL::BLUE = 0;
uint32 MATERIAL::BOAT = 0;
uint32 MATERIAL::GROUND = 0;
uint32 MATERIAL::WATER = 0;
uint32 MATERIAL::WALL_STANDARD = 0;
uint32 MATERIAL::CREW_STANDARD = 0;
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
	BLACK				= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)		, 256.0f, -1, SHADER::STANDARD_MATERIAL);
	WHITE				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)		, 256.0f, -1, SHADER::STANDARD_MATERIAL);
	RED					= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 256.0f, -1, SHADER::STANDARD_MATERIAL);
	RED_1				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 128.0f, -1, SHADER::STANDARD_MATERIAL);
	RED_2				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 96.0f, -1, SHADER::STANDARD_MATERIAL);
	RED_3				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 32.0f, -1, SHADER::STANDARD_MATERIAL);
	RED_4				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 16.0f, -1, SHADER::STANDARD_MATERIAL);
	GREEN				= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)		, 256.0f, -1, SHADER::STANDARD_MATERIAL);
	BLUE				= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)		, 256.0f, -1, SHADER::STANDARD_MATERIAL);
	BOAT				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 256.0f, -1, SHADER::STANDARD_MATERIAL);
	GROUND				= ResourceHandler::RegisterMaterial(glm::vec4(0.471f, 0.282f, 0.11f, 1.0f)	, 256.0f, -1, SHADER::STANDARD_MATERIAL);
	CREW_STANDARD		= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)		, 256.0f, -1, SHADER::STANDARD_MATERIAL);
	WALL_STANDARD		= ResourceHandler::RegisterWallMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)		, 256.0f, -1);
	WATER				= ResourceHandler::RegisterWaterMaterial(TEXTURE::WATER_DISTORTION, TEXTURE::WATER_NORMAL);
	SINGLE_BED			= ResourceHandler::RegisterMaterial(TEXTURE::SINGLE_BED, -1, SHADER::STANDARD_MATERIAL);
	BUNK_BED			= ResourceHandler::RegisterMaterial(TEXTURE::BUNK_BED, -1, SHADER::STANDARD_MATERIAL);
}