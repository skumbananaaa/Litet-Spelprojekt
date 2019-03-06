#pragma once
#include <EnginePch.h>

class API TEXTURE
{
	friend class ResourceHandler;

public:
	static uint32 BLOOD;
	static uint32 BLOOD_NORMAL;
	static uint32 SHIP;
	static uint32 SHIP_NORMAL;
	static uint32 HDR;
	static uint32 SINGLE_BED;
	static uint32 BUNK_BED;
	static uint32 CUBOARD;
	static uint32 TABLE;
	static uint32 TOILET;
	static uint32 INSTRUMENT_1;
	static uint32 INSTRUMENT_2;
	static uint32 ICON_INJURY_BONE;
	static uint32 ICON_INJURY_BURN;
	static uint32 ICON_INJURY_SMOKE;
	static uint32 ICON_INJURY_BLEED;
	static uint32 ICON_SKILL_FIRE;
	static uint32 ICON_SKILL_MEDIC;
	static uint32 ICON_SKILL_STRENGTH;
	static uint32 ICON_CIRCLE;
	static uint32 ICON_CHECKED;
	static uint32 ICON_CROSSED;
	static uint32 SMOKE;
	static uint32 SJOFAN_DIFF;
	static uint32 WALL1;
	static uint32 FLOOR1;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};