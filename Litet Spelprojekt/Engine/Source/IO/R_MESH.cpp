#include <EnginePch.h>
#include <IO/MESH.h>
#include <IO/ResourceHandler.h>
#include <Graphics/Geometry/IndexedMesh.h>

uint32 MESH::QUAD = 0;
uint32 MESH::SHIP = 0;
uint32 MESH::CLIFF_3_LOW = 0;
uint32 MESH::CUBE = 0;
uint32 MESH::CUBE_INV_NORMALS = 0;
uint32 MESH::CUBE_OBJ = 0;
uint32 MESH::CHAIR = 0;
uint32 MESH::SPHERE = 0;
uint32 MESH::BED_BUNK = 0;
uint32 MESH::BED_SINGLE = 0;
uint32 MESH::INSTRUMENT_1 = 0;
uint32 MESH::INSTRUMENT_2 = 0;
uint32 MESH::LAMP = 0;
uint32 MESH::ANIMATED_MODEL = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void MESH::RegisterResourcesPreLoading()
{

}

void MESH::RegisterResources()
{
	QUAD					= ResourceHandler::RegisterMesh(IndexedMesh::CreateQuad());
	CUBE					= ResourceHandler::RegisterMesh(IndexedMesh::CreateCube());
	CUBE_INV_NORMALS		= ResourceHandler::RegisterMesh(IndexedMesh::CreateCubeInvNormals());
	SHIP					= ResourceHandler::RegisterMesh("ship1.obj");
	CLIFF_3_LOW				= ResourceHandler::RegisterMesh("cliff_3_low.obj");
	CUBE_OBJ				= ResourceHandler::RegisterMesh("cube.obj");
	SPHERE					= ResourceHandler::RegisterMesh("sphere.obj");
	CHAIR					= ResourceHandler::RegisterMesh("Chair.obj");
	BED_BUNK				= ResourceHandler::RegisterMesh("BunkBed.obj");
	BED_SINGLE				= ResourceHandler::RegisterMesh("SingleBed1.obj");
	INSTRUMENT_1			= ResourceHandler::RegisterMesh("Instrument1.obj");
	INSTRUMENT_2			= ResourceHandler::RegisterMesh("Instrument2.obj");
	LAMP					= ResourceHandler::RegisterMesh("Lamp.obj");
	ANIMATED_MODEL			= ResourceHandler::RegisterAnimatedMesh("bob.dae");
}