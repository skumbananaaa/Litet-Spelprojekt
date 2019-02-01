#pragma once
#include <EnginePch.h>
#include <World/ResourceHandler.h>

class API Resources
{
public:
	static uint32 MESH_QUAD;
	static uint32 MESH_SHIP;
	static uint32 MESH_CLIFF_3_LOW;
	static uint32 MESH_CUBE;

public:
	static uint32 MATERIAL_WHITE;

public:
	static void RegisterResources();
};