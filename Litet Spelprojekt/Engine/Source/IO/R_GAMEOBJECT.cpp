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
uint32 GAMEOBJECT::CUBOARD = 0;
uint32 GAMEOBJECT::TABLE = 0;
uint32 GAMEOBJECT::TOILET = 0;
uint32 GAMEOBJECT::DOOR = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void GAMEOBJECT::RegisterResourcesPreLoading()
{

}

void GAMEOBJECT::RegisterResources()
{
	CHAIR						= ResourceHandler::RegisterGameObject("Chair", MESH::CHAIR, MATERIAL::CHAIR);
	BED_BUNK					= ResourceHandler::RegisterGameObject("Bunk Bed", MESH::BED_BUNK, MATERIAL::BUNK_BED);
	BED_SINGLE					= ResourceHandler::RegisterGameObject("Single Bed", MESH::BED_SINGLE, MATERIAL::SINGLE_BED);
	INSTRUMENT_1				= ResourceHandler::RegisterGameObject("Instrument 1", MESH::INSTRUMENT_1, MATERIAL::INSTRUMENT_1);
	INSTRUMENT_2				= ResourceHandler::RegisterGameObject("Instrument 2", MESH::INSTRUMENT_2, MATERIAL::INSTRUMENT_2);
	LAMP						= ResourceHandler::RegisterGameObject("Lamp", MESH::LAMP, MATERIAL::LAMP);
	FIREALARM					= ResourceHandler::RegisterGameObject("Firealarm", MESH::LAMP, MATERIAL::RED);
	CUBOARD						= ResourceHandler::RegisterGameObject("Cuboard", MESH::CUBOARD, MATERIAL::CUBOARD);
	TABLE						= ResourceHandler::RegisterGameObject("Table", MESH::TABLE, MATERIAL::TABLE);
	TOILET						= ResourceHandler::RegisterGameObject("Toilet", MESH::TOILET, MATERIAL::TOILET);
	DOOR						= ResourceHandler::RegisterGameObject("Door", MESH::DOOR, MATERIAL::WHITE);
}