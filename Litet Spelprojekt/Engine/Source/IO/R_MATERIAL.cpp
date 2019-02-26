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
uint32 MATERIAL::OCEAN_BLUE = 0;
uint32 MATERIAL::BOAT = 0;
uint32 MATERIAL::GROUND = 0;
uint32 MATERIAL::WATER_OUTDOOR = 0;
uint32 MATERIAL::WATER_INDOOR = 0;
uint32 MATERIAL::WALL_STANDARD = 0;
uint32 MATERIAL::CREW_STANDARD = 0;
uint32 MATERIAL::SINGLE_BED = 0;
uint32 MATERIAL::BUNK_BED = 0;
uint32 MATERIAL::INSTRUMENT_1 = 0;
uint32 MATERIAL::INSTRUMENT_2 = 0;
uint32 MATERIAL::LAMP = 0;
uint32 MATERIAL::CHAIR = 0;
uint32 MATERIAL::CUBOARD = 0;
uint32 MATERIAL::TABLE = 0;
uint32 MATERIAL::TOILET = 0;
uint32 MATERIAL::ANIMATED_MODEL = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void MATERIAL::RegisterResourcesPreLoading()
{

}

void MATERIAL::RegisterResources()
{
	OCEAN_BLUE			= ResourceHandler::RegisterMaterial(glm::vec4(0.09f, 0.34f, 0.49f, 1.0f)	, 256.0f, -1, SHADER::STANDARD_MATERIAL);
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
	CHAIR				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f)		, 256.0f, -1, SHADER::STANDARD_MATERIAL);
	LAMP				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f)		, 256.0f, -1, SHADER::STANDARD_MATERIAL);
	WALL_STANDARD		= ResourceHandler::RegisterWallMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)		, 256.0f, -1);
	WATER_OUTDOOR		= ResourceHandler::RegisterWaterOutdoorMaterial(TEXTURE::WATER_DISTORTION, TEXTURE::WATER_NORMAL);
	WATER_INDOOR		= ResourceHandler::RegisterWaterIndoorMaterial(TEXTURE::WATER_DISTORTION, TEXTURE::WATER_NORMAL);
	INSTRUMENT_1		= ResourceHandler::RegisterMaterial(TEXTURE::INSTRUMENT_1, -1, SHADER::STANDARD_MATERIAL);
	INSTRUMENT_2		= ResourceHandler::RegisterMaterial(TEXTURE::INSTRUMENT_2, -1, SHADER::STANDARD_MATERIAL);
	SINGLE_BED			= ResourceHandler::RegisterMaterial(TEXTURE::SINGLE_BED, -1, SHADER::STANDARD_MATERIAL);
	BUNK_BED			= ResourceHandler::RegisterMaterial(TEXTURE::BUNK_BED, -1, SHADER::STANDARD_MATERIAL);
	CUBOARD				= ResourceHandler::RegisterMaterial(TEXTURE::CUBOARD, -1, SHADER::STANDARD_MATERIAL);
	TABLE				= ResourceHandler::RegisterMaterial(TEXTURE::TABLE, -1, SHADER::STANDARD_MATERIAL);
	TOILET				= ResourceHandler::RegisterMaterial(TEXTURE::TOILET, -1, SHADER::STANDARD_MATERIAL);
	
	ANIMATED_MODEL		= ResourceHandler::RegisterMaterial(TEXTURE::SJOFAN_DIFF, -1, SHADER::ANIMATION);
}