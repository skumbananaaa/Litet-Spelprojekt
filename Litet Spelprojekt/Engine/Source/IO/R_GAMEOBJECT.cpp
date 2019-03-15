#include <EnginePch.h>
#include <IO/GAMEOBJECT.h>
#include <IO/ResourceHandler.h>
#include <Graphics/GameObject.h>
#include <World/Scenarios/Fire/FireAlarm.h>

uint32 GAMEOBJECT::CHAIR = 0;
uint32 GAMEOBJECT::BED_BUNK = 0;
uint32 GAMEOBJECT::BED_SINGLE = 0;
uint32 GAMEOBJECT::INSTRUMENT_1 = 0;
uint32 GAMEOBJECT::INSTRUMENT_2 = 0;
uint32 GAMEOBJECT::LAMP = 0;
uint32 GAMEOBJECT::FIREALARM = 0;
uint32 GAMEOBJECT::CUPBOARD = 0;
uint32 GAMEOBJECT::TABLE = 0;
uint32 GAMEOBJECT::TOILET = 0;
uint32 GAMEOBJECT::DOOR = 0;
uint32 GAMEOBJECT::FLOOR = 0;
uint32 GAMEOBJECT::OVEN = 0;
uint32 GAMEOBJECT::SHELF_EMPTY = 0;
uint32 GAMEOBJECT::SHELF_AMMUNITION = 0;
uint32 GAMEOBJECT::GENERATOR = 0;
uint32 GAMEOBJECT::FIRE_EXTINGUISHER = 0;
uint32 GAMEOBJECT::FIRESPRINKLER = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void GAMEOBJECT::RegisterResourcesPreLoading()
{

}

void GAMEOBJECT::RegisterResources()
{
	CHAIR						= ResourceHandler::RegisterGameObject("Stol", MESH::CHAIR, MATERIAL::CHAIR);
	BED_BUNK					= ResourceHandler::RegisterGameObject("Bunkbed", MESH::BED_BUNK, MATERIAL::BUNK_BED);
	BED_SINGLE					= ResourceHandler::RegisterGameObject("Singlebed", MESH::BED_SINGLE, MATERIAL::SINGLE_BED);
	INSTRUMENT_1				= ResourceHandler::RegisterGameObject("Instrument 1", MESH::INSTRUMENT_1, MATERIAL::INSTRUMENT_1);
	INSTRUMENT_2				= ResourceHandler::RegisterGameObject("Instrument 2", MESH::INSTRUMENT_2, MATERIAL::INSTRUMENT_2);
	LAMP						= ResourceHandler::RegisterGameObject("Lampa", MESH::LAMP, MATERIAL::LAMP);
	FIREALARM					= ResourceHandler::RegisterGameObject("Brandvarnare", MESH::LAMP, MATERIAL::FIRE_RELATED);
	CUPBOARD					= ResourceHandler::RegisterGameObject("Skåp", MESH::CUPBOARD, MATERIAL::CUPBOARD);
	TABLE						= ResourceHandler::RegisterGameObject("Bord", MESH::TABLE, MATERIAL::TABLE);
	TOILET						= ResourceHandler::RegisterGameObject("Toalett", MESH::TOILET, MATERIAL::TOILET);
	DOOR						= ResourceHandler::RegisterGameObject("Dörr", MESH::DOOR, MATERIAL::WHITE);
	FLOOR						= ResourceHandler::RegisterGameObject("Golv", MESH::CUBE);
	OVEN						= ResourceHandler::RegisterGameObject("Ugn", MESH::OVEN, MATERIAL::OVEN);
	SHELF_EMPTY					= ResourceHandler::RegisterGameObject("Hylla", MESH::SHELF_EMPTY, MATERIAL::SHELF_EMPTY);
	SHELF_AMMUNITION			= ResourceHandler::RegisterGameObject("Ammunitionshylla", MESH::SHELF_AMMUNITION, MATERIAL::SHELF_AMMUNITION);
	GENERATOR					= ResourceHandler::RegisterGameObject("Generator", MESH::GENERATOR, MATERIAL::GENERATOR);
	FIRE_EXTINGUISHER			= ResourceHandler::RegisterGameObject("Extinguisher", MESH::FIRE_EXTINGUISHER, MATERIAL::FIRE_EXTINGUISHER);
	FIRESPRINKLER				= ResourceHandler::RegisterGameObject("Sprinkler", MESH::FIRESPRINKLER, MATERIAL::FIRESPRINKLER);
}