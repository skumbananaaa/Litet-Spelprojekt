#pragma once
#include <EnginePch.h>

struct MESH_DESC
{
	uint32 mesh;
	std::string name;
};

class API MESH
{
public:
	static uint32 QUAD;
	static uint32 SHIP;
	static uint32 CLIFF_3_LOW;
	static uint32 CUBE;
	static uint32 CUBE_OBJ;
	static uint32 CHAIR;

public:
	static void RegisterResources();
};