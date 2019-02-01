#include <EnginePch.h>
#include <World\Resources.h>

uint32 Resources::MESH_QUAD = 0;
uint32 Resources::MESH_SHIP = 0;
uint32 Resources::MESH_CLIFF_3_LOW = 0;
uint32 Resources::MESH_CUBE = 0;

uint32 Resources::MATERIAL_WHITE = 0;

void Resources::RegisterResources()
{
	Resources::MESH_QUAD			= ResourceHandler::RegisterMesh(IndexedMesh::CreateQuad());
	Resources::MESH_CUBE			= ResourceHandler::RegisterMesh(IndexedMesh::CreateCube());
	Resources::MESH_SHIP			= ResourceHandler::RegisterMesh("ship.obj");
	Resources::MESH_CLIFF_3_LOW		= ResourceHandler::RegisterMesh("cliff_3_low.obj");

	Resources::MATERIAL_WHITE		= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 1.0F, 1.0F, 1.0F), "");
}