#include <EnginePch.h>
#include <IO/MESH.h>
#include <IO/ResourceHandler.h>

uint32 MESH::QUAD = 0;
uint32 MESH::SHIP = 0;
uint32 MESH::CLIFF_3_LOW = 0;
uint32 MESH::CUBE = 0;
uint32 MESH::CUBE_OBJ = 0;
uint32 MESH::CHAIR = 0;
uint32 MESH::SPHERE = 0;
uint32 MESH::BED_BUNK = 0;
uint32 MESH::BED_SINGLE = 0;

void MESH::RegisterResources()
{
	QUAD					= ResourceHandler::RegisterMesh(IndexedMesh::CreateQuad(), false);
	CUBE					= ResourceHandler::RegisterMesh(IndexedMesh::CreateCube(), false);
	SHIP					= ResourceHandler::RegisterMesh("ship.obj", false);
	CLIFF_3_LOW				= ResourceHandler::RegisterMesh("cliff_3_low.obj", false);
	CUBE_OBJ				= ResourceHandler::RegisterMesh("cube.obj", false);
	SPHERE					= ResourceHandler::RegisterMesh("sphere.obj", false);
	CHAIR					= ResourceHandler::RegisterMesh("Chair.obj");
	BED_BUNK				= ResourceHandler::RegisterMesh("BunkBed.obj");
	BED_SINGLE				= ResourceHandler::RegisterMesh("SingleBed.obj");
}