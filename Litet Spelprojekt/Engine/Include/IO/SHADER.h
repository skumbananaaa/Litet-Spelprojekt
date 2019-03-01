#pragma once
#include <EnginePch.h>

class API SHADER
{
	friend class ResourceHandler;

public:
	static uint32 GUI;
	static uint32 FONT;
	static uint32 DEPTH_PRE_PASS;
	static uint32 FORWARD_PASS;
	static uint32 SKYBOX_PASS;
	static uint32 ORTHOGRAPHIC;
	static uint32 WALL_MATERIAL;
	static uint32 FLOOR;
	static uint32 WATER_OUTDOOR_MATERIAL;
	static uint32 WATER_INDOOR_MATERIAL;
	static uint32 STANDARD_MATERIAL;
	static uint32 PARTICLES;
	static uint32 MESH_PARTICLES;
	static uint32 EQUIREC_TO_CUBEMAP;
	static uint32 ANIMATION;
	static uint32 ANIMATION_DEPTH_PRE_PASS;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};