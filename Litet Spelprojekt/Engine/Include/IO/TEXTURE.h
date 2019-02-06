#pragma once
#include <EnginePch.h>

class API TEXTURE
{
public:
	static uint32 BLOOD;
	static uint32 BLOOD_NORMAL;
	static uint32 SHIP;
	static uint32 SHIP_NORMAL;
	static uint32 WATER_DISTORTION;
	static uint32 WATER_NORMAL;
	static uint32 HDR;

public:
	static void RegisterResources();
};