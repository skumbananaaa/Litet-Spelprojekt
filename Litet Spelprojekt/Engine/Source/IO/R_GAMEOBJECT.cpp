#include <EnginePch.h>
#include <IO/GAMEOBJECT.h>
#include <IO/ResourceHandler.h>

uint32 GAMEOBJECT::CHAIR = 0;
uint32 GAMEOBJECT::BED_BUNK = 0;
uint32 GAMEOBJECT::BED_SINGLE = 0;
uint32 GAMEOBJECT::INSTRUMENT_1 = 0;
uint32 GAMEOBJECT::INSTRUMENT_2 = 0;
uint32 GAMEOBJECT::LAMP = 0;

void GAMEOBJECT::RegisterResources()
{
	CHAIR						= ResourceHandler::RegisterGameObject("Chair", MESH::CHAIR, MATERIAL::WHITE);
	BED_BUNK					= ResourceHandler::RegisterGameObject("Bunk Bed", MESH::BED_BUNK, MATERIAL::BUNK_BED);
	BED_SINGLE					= ResourceHandler::RegisterGameObject("Single Bed", MESH::BED_SINGLE, MATERIAL::SINGLE_BED);
	INSTRUMENT_1				= ResourceHandler::RegisterGameObject("Instrument 1", MESH::INSTRUMENT_1, MATERIAL::WHITE);
	INSTRUMENT_2				= ResourceHandler::RegisterGameObject("Instrument 2", MESH::INSTRUMENT_2, MATERIAL::WHITE);
	LAMP						= ResourceHandler::RegisterGameObject("Lamp", MESH::LAMP, MATERIAL::WHITE);
}