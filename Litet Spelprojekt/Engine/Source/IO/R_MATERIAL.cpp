#include <EnginePch.h>
#include <IO/MATERIAL.h>
#include <IO/ResourceHandler.h>

uint32 MATERIAL::BLACK = 0;
uint32 MATERIAL::WHITE = 0;
uint32 MATERIAL::ARTILLERY = 0;
uint32 MATERIAL::DOOR_FRAME = 0;
uint32 MATERIAL::DOOR_RED = 0;
uint32 MATERIAL::DOOR_GREEN = 0;
uint32 MATERIAL::DOOR_BLUE = 0;
uint32 MATERIAL::DOOR_YELLOW = 0;
uint32 MATERIAL::FIRE_RELATED = 0;
uint32 MATERIAL::LADDER = 0;
uint32 MATERIAL::OCEAN_BLUE = 0;
uint32 MATERIAL::BOAT = 0;
uint32 MATERIAL::GROUND = 0;
uint32 MATERIAL::WATER_OUTDOOR = 0;
uint32 MATERIAL::WATER_INDOOR = 0;
uint32 MATERIAL::WALL_STANDARD = 0;
uint32 MATERIAL::BULKHEADS_STANDARD = 0;
uint32 MATERIAL::CREW_STANDARD = 0;
uint32 MATERIAL::SINGLE_BED = 0;
uint32 MATERIAL::BUNK_BED = 0;
uint32 MATERIAL::INSTRUMENT_1 = 0;
uint32 MATERIAL::INSTRUMENT_2 = 0;
uint32 MATERIAL::LAMP = 0;
uint32 MATERIAL::CHAIR = 0;
uint32 MATERIAL::CUPBOARD = 0;
uint32 MATERIAL::TABLE = 0;
uint32 MATERIAL::TOILET = 0;
uint32 MATERIAL::ANIMATED_MODEL = 0;
uint32 MATERIAL::FLOOR_NORMAL = 0;
uint32 MATERIAL::FLOOR_SICKBAY1 = 0;
uint32 MATERIAL::FLOOR_TOILET1 = 0;
uint32 MATERIAL::FLOOR_MACHINE1 = 0;
uint32 MATERIAL::FLOOR_MACHINE2 = 0;
uint32 MATERIAL::FLOOR_MACHINE3 = 0;
uint32 MATERIAL::FLOOR_AMMUNITION1 = 0;
uint32 MATERIAL::FLOOR_AMMUNITION2 = 0;
uint32 MATERIAL::FLOOR_AMMUNITION3 = 0;
uint32 MATERIAL::FLOOR_KITCHEN1 = 0;
uint32 MATERIAL::FLOOR_DINING1 = 0;
uint32 MATERIAL::FLOOR_CABOOSE1 = 0;
uint32 MATERIAL::OVEN = 0;
uint32 MATERIAL::SHELF_EMPTY = 0;
uint32 MATERIAL::SHELF_AMMUNITION = 0;
uint32 MATERIAL::GENERATOR = 0;
uint32 MATERIAL::FIRE_EXTINGUISHER = 0;
uint32 MATERIAL::FIRESPRINKLER = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void MATERIAL::RegisterResourcesPreLoading()
{
	WATER_OUTDOOR		= ResourceHandler::RegisterWaterOutdoorMaterial();
	BOAT				= ResourceHandler::RegisterMaterial(glm::vec4(2.2f, 2.5f, 2.4f, 1.0f), 256.0f, SHADER::STANDARD_MATERIAL);
}

void MATERIAL::RegisterResources()
{
	OCEAN_BLUE			= ResourceHandler::RegisterMaterial(glm::vec4(0.09f, 0.34f, 0.49f, 1.0f)	, 256.0f, SHADER::STANDARD_MATERIAL);
	BLACK				= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	WHITE				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	ARTILLERY			= ResourceHandler::RegisterMaterial(glm::vec4(0.545f, 0.271, 0.075f, 1.0f)	, 256.0f, SHADER::STANDARD_MATERIAL);
	DOOR_FRAME			= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	DOOR_RED			= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	DOOR_GREEN			= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	DOOR_BLUE			= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	DOOR_YELLOW			= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	FIRE_RELATED		= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	LADDER				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	GROUND				= ResourceHandler::RegisterMaterial(glm::vec4(0.471f, 0.282f, 0.11f, 1.0f)	, 256.0f, SHADER::STANDARD_MATERIAL);
	CREW_STANDARD		= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	WALL_STANDARD		= ResourceHandler::RegisterWallMaterial(TEXTURE::WALL1						, 256.0f);
	BULKHEADS_STANDARD	= ResourceHandler::RegisterWallMaterial(TEXTURE::WALL1						, 256.0f);
	WATER_INDOOR		= ResourceHandler::RegisterWaterIndoorMaterial();
	SINGLE_BED			= ResourceHandler::RegisterMaterial(TEXTURE::SINGLE_BED, SHADER::STANDARD_MATERIAL);
	BUNK_BED			= ResourceHandler::RegisterMaterial(TEXTURE::BUNK_BED, SHADER::STANDARD_MATERIAL);
	ANIMATED_MODEL		= ResourceHandler::RegisterMaterial(TEXTURE::SJOFAN_DIFF, SHADER::ANIMATION);
	INSTRUMENT_1		= ResourceHandler::RegisterMaterial(TEXTURE::INSTRUMENT_1, SHADER::STANDARD_MATERIAL);
	INSTRUMENT_2		= ResourceHandler::RegisterMaterial(TEXTURE::INSTRUMENT_2, SHADER::STANDARD_MATERIAL);
	CUPBOARD			= ResourceHandler::RegisterMaterial(TEXTURE::CUPBOARD, SHADER::STANDARD_MATERIAL);
	TABLE				= ResourceHandler::RegisterMaterial(TEXTURE::TABLE, SHADER::STANDARD_MATERIAL);
	TOILET				= ResourceHandler::RegisterMaterial(TEXTURE::TOILET, SHADER::STANDARD_MATERIAL);
	CHAIR				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	LAMP				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f)		, 256.0f, SHADER::STANDARD_MATERIAL);
	FLOOR_NORMAL		= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_NORMAL, SHADER::FLOOR);
	FLOOR_SICKBAY1		= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_SICKBAY1, SHADER::FLOOR);
	FLOOR_TOILET1		= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_TOILET1, SHADER::FLOOR);
	FLOOR_MACHINE1		= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_MACHINE1, SHADER::FLOOR);
	FLOOR_MACHINE2		= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_MACHINE2, SHADER::FLOOR);
	FLOOR_MACHINE3		= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_MACHINE3, SHADER::FLOOR);
	FLOOR_AMMUNITION1	= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_AMMUNITION1, SHADER::FLOOR);
	FLOOR_AMMUNITION2	= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_AMMUNITION2, SHADER::FLOOR);
	FLOOR_AMMUNITION3	= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_AMMUNITION3, SHADER::FLOOR);
	FLOOR_KITCHEN1		= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_KITCHEN1, SHADER::FLOOR);
	FLOOR_DINING1		= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_DINING1, SHADER::FLOOR);
	FLOOR_CABOOSE1		= ResourceHandler::RegisterMaterial(TEXTURE::FLOOR_CABOOSE1, SHADER::FLOOR);
	OVEN				= ResourceHandler::RegisterMaterial(TEXTURE::OVEN, SHADER::STANDARD_MATERIAL);
	SHELF_AMMUNITION	= ResourceHandler::RegisterMaterial(TEXTURE::SHELF_AMMUNITION, SHADER::STANDARD_MATERIAL);
	SHELF_EMPTY			= ResourceHandler::RegisterMaterial(TEXTURE::SHELF_EMPTY, SHADER::STANDARD_MATERIAL);
	GENERATOR			= ResourceHandler::RegisterMaterial(TEXTURE::GENERATOR, SHADER::STANDARD_MATERIAL);
	FIRE_EXTINGUISHER	= ResourceHandler::RegisterMaterial(TEXTURE::FIRE_EXTINGUISHER, SHADER::STANDARD_MATERIAL);
	FIRESPRINKLER		= ResourceHandler::RegisterMaterial(TEXTURE::FIRESPRINKLER, SHADER::STANDARD_MATERIAL);
}