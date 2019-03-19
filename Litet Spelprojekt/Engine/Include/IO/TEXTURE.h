#pragma once
#include <EnginePch.h>

class API TEXTURE
{
	friend class ResourceHandler;

public:
	static uint32 SHIP;
	static uint32 HDR;
	static uint32 SINGLE_BED;
	static uint32 BUNK_BED;
	static uint32 CUPBOARD;
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
	static uint32 ICON_RED_DOOR;
	static uint32 ICON_YELLOW_DOOR;
	static uint32 ICON_BLUE_DOOR;
	static uint32 ICON_GREEN_DOOR;
	static uint32 SMOKE;
	static uint32 SJOFAN_DIFF;
	static uint32 SJOFAN_DIFF_EXTINGUISH;
	static uint32 CREW_INJURY;
	static uint32 WALL1;
	static uint32 FLOOR_NORMAL;
	static uint32 FLOOR_SICKBAY1;
	static uint32 FLOOR_TOILET1;
	static uint32 FLOOR_MACHINE1;
	static uint32 FLOOR_MACHINE2;
	static uint32 FLOOR_MACHINE3;
	static uint32 FLOOR_AMMUNITION1;
	static uint32 FLOOR_AMMUNITION2;
	static uint32 FLOOR_AMMUNITION3;
	static uint32 FLOOR_KITCHEN1;
	static uint32 FLOOR_DINING1;
	static uint32 FLOOR_CABOOSE1;
	static uint32 FLOOR_EXT_NORMAL;
	static uint32 FLOOR_EXT_SICKBAY1;
	static uint32 FLOOR_EXT_TOILET1;
	static uint32 FLOOR_EXT_MACHINE1;
	static uint32 FLOOR_EXT_MACHINE2;
	static uint32 FLOOR_EXT_MACHINE3;
	static uint32 FLOOR_EXT_AMMUNITION1;
	static uint32 FLOOR_EXT_AMMUNITION2;
	static uint32 FLOOR_EXT_AMMUNITION3;
	static uint32 FLOOR_EXT_KITCHEN1;
	static uint32 FLOOR_EXT_DINING1;
	static uint32 FLOOR_EXT_CABOOSE1;
	static uint32 OVEN;
	static uint32 LOADING_CONTROLS;
	static uint32 TUTORIAL_SCREEN_1;
	static uint32 TUTORIAL_SCREEN_2;
	static uint32 TUTORIAL_SCREEN_3;
	static uint32 SHELF_AMMUNITION;
	static uint32 SHELF_EMPTY;
	static uint32 GENERATOR;
	static uint32 FIRE_EXTINGUISHER;
	static uint32 FIRESPRINKLER;
	static uint32 ICON_NOTIFICATION_TRIANGLE;
	static uint32 ICON_LOGBOOK;
	static uint32 LOOK_AT;

private:
	static void RegisterResourcesPreLoading();
	static void RegisterResources();
};