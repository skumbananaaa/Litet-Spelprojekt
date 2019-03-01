#include <EnginePch.h>
#include <IO/TEXTURE.h>
#include <IO/ResourceHandler.h>
#include <Graphics/Textures/Texture2D.h>

uint32 TEXTURE::BLOOD = 0;
uint32 TEXTURE::BLOOD_NORMAL = 0;
uint32 TEXTURE::SHIP = 0;
uint32 TEXTURE::SHIP_NORMAL = 0;
uint32 TEXTURE::HDR = 0;
uint32 TEXTURE::SINGLE_BED = 0;
uint32 TEXTURE::BUNK_BED = 0;
uint32 TEXTURE::CUBOARD = 0;
uint32 TEXTURE::TABLE = 0;
uint32 TEXTURE::TOILET = 0;
uint32 TEXTURE::INSTRUMENT_1 = 0;
uint32 TEXTURE::INSTRUMENT_2 = 0;
uint32 TEXTURE::ICON_INJURY_BONE = 0;
uint32 TEXTURE::ICON_INJURY_BURN = 0;
uint32 TEXTURE::ICON_INJURY_SMOKE = 0;
uint32 TEXTURE::ICON_SKILL_FIRE = 0;
uint32 TEXTURE::ICON_SKILL_MEDIC = 0;
uint32 TEXTURE::ICON_SKILL_STRENGTH = 0;
uint32 TEXTURE::ICON_CIRCLE = 0;
uint32 TEXTURE::ICON_CHECKED = 0;
uint32 TEXTURE::ICON_CROSSED = 0;
uint32 TEXTURE::SMOKE = 0;
uint32 TEXTURE::SJOFAN_DIFF = 0;
uint32 TEXTURE::WALL1 = 0;
uint32 TEXTURE::FLOOR1 = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void TEXTURE::RegisterResourcesPreLoading()
{
	TextureParams params = {};
	params.MinFilter = TEX_PARAM_LINEAR;
	params.MagFilter = TEX_PARAM_LINEAR;
	params.Wrap = TEX_PARAM_EDGECLAMP;
	HDR					= ResourceHandler::RegisterTexture2D("SkyBoxTextures/ocean.hdr", TEX_FORMAT_RGB16F, true, false, params);
}

void TEXTURE::RegisterResources()
{
	TextureParams params = {};
	params.Wrap = TEX_PARAM_REPEAT;
	params.MinFilter = TEX_PARAM_LINEAR;
	params.MagFilter = TEX_PARAM_LINEAR;

	BLOOD				= ResourceHandler::RegisterTexture2D("blood.png", TEX_FORMAT_RGBA, true, false, params);
	BLOOD_NORMAL		= ResourceHandler::RegisterTexture2D("bloodNormalMap.png", TEX_FORMAT_RGBA, true, false, params);
	SHIP				= ResourceHandler::RegisterTexture2D("ship.jpg", TEX_FORMAT_RGBA);
	SHIP_NORMAL			= ResourceHandler::RegisterTexture2D("shipNormalMap.png", TEX_FORMAT_RGBA);
	SINGLE_BED			= ResourceHandler::RegisterTexture2D("uv_Single_Bed_Texture_Map.jpg", TEX_FORMAT_RGBA);
	INSTRUMENT_1		= ResourceHandler::RegisterTexture2D("uv_Instrument1.jpg", TEX_FORMAT_RGBA);
	INSTRUMENT_2		= ResourceHandler::RegisterTexture2D("uv_Instrument2.jpg", TEX_FORMAT_RGBA);
	BUNK_BED			= ResourceHandler::RegisterTexture2D("uv_bunk_Bed_Texture.jpg", TEX_FORMAT_RGBA);
	CUBOARD				= ResourceHandler::RegisterTexture2D("uv_Cuboard.jpg", TEX_FORMAT_RGBA);
	TABLE				= ResourceHandler::RegisterTexture2D("uv_Table.jpg", TEX_FORMAT_RGBA);
	TOILET				= ResourceHandler::RegisterTexture2D("uv_Toilet.jpg", TEX_FORMAT_RGBA);
	SMOKE				= ResourceHandler::RegisterTexture2D("smoke.png", TEX_FORMAT_RGBA, true, false, params);
	SJOFAN_DIFF			= ResourceHandler::RegisterTexture2D("CrewMember_diff.jpg", TEX_FORMAT_RGBA, true, false, params);
	WALL1				= ResourceHandler::RegisterTexture2D("wallTexture.jpg", TEX_FORMAT_RGBA, true, false, params);
	FLOOR1				= ResourceHandler::RegisterTexture2D("floorTexture.jpg", TEX_FORMAT_RGBA, true, false, params);

	ICON_INJURY_BONE	= ResourceHandler::RegisterTexture2D("Icons/InjuryBone.png", TEX_FORMAT_RGBA, false, true);
	ICON_INJURY_BURN	= ResourceHandler::RegisterTexture2D("Icons/InjuryBurn.png", TEX_FORMAT_RGBA, false, true);
	ICON_INJURY_SMOKE	= ResourceHandler::RegisterTexture2D("Icons/InjurySmoke.png", TEX_FORMAT_RGBA, false, true);
	ICON_SKILL_FIRE		= ResourceHandler::RegisterTexture2D("Icons/SkillFire.png", TEX_FORMAT_RGBA, false, true);
	ICON_SKILL_MEDIC	= ResourceHandler::RegisterTexture2D("Icons/SkillMedic.png", TEX_FORMAT_RGBA, false, true);
	ICON_SKILL_STRENGTH = ResourceHandler::RegisterTexture2D("Icons/SkillStrength.png", TEX_FORMAT_RGBA, false, true);
	ICON_CIRCLE			= ResourceHandler::RegisterTexture2D("Icons/Circle.png", TEX_FORMAT_RGBA, false, true);
	ICON_CHECKED		= ResourceHandler::RegisterTexture2D("Icons/Checked.png", TEX_FORMAT_RGBA, false, true);
	ICON_CROSSED		= ResourceHandler::RegisterTexture2D("Icons/Crossed.png", TEX_FORMAT_RGBA, false, true);
}