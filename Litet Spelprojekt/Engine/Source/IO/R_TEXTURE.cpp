#include <EnginePch.h>
#include <IO/TEXTURE.h>
#include <IO/ResourceHandler.h>
#include <Graphics/Textures/Texture2D.h>

uint32 TEXTURE::BLOOD = 0;
uint32 TEXTURE::BLOOD_NORMAL = 0;
uint32 TEXTURE::SHIP = 0;
uint32 TEXTURE::SHIP_NORMAL = 0;
uint32 TEXTURE::WATER_DISTORTION = 0;
uint32 TEXTURE::WATER_NORMAL = 0;
uint32 TEXTURE::HDR = 0;
uint32 TEXTURE::SINGLE_BED = 0;
uint32 TEXTURE::BUNK_BED = 0;
uint32 TEXTURE::ICON_INJURY_BONE = 0;
uint32 TEXTURE::ICON_INJURY_BURN = 0;
uint32 TEXTURE::ICON_INJURY_SMOKE = 0;
uint32 TEXTURE::ICON_SKILL_FIRE = 0;
uint32 TEXTURE::ICON_SKILL_MEDIC = 0;
uint32 TEXTURE::ICON_SKILL_STRENGTH = 0;
uint32 TEXTURE::ICON_CIRCLE = 0;
uint32 TEXTURE::SMOKE = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void TEXTURE::RegisterResourcesPreLoading()
{
	
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
	BUNK_BED			= ResourceHandler::RegisterTexture2D("uv_bunk_Bed_Texture.jpg", TEX_FORMAT_RGBA);
	WATER_DISTORTION	= ResourceHandler::RegisterTexture2D("waterDUDV.png", TEX_FORMAT_RGBA, true, false, params);
	WATER_NORMAL		= ResourceHandler::RegisterTexture2D("waterNormalMap.png", TEX_FORMAT_RGBA, true, false, params);
	SMOKE				= ResourceHandler::RegisterTexture2D("smoke.png", TEX_FORMAT_RGBA, true, false, params);

	ICON_INJURY_BONE	= ResourceHandler::RegisterTexture2D("Icons/InjuryBone.png", TEX_FORMAT_RGBA, false, true);
	ICON_INJURY_BURN	= ResourceHandler::RegisterTexture2D("Icons/InjuryBurn.png", TEX_FORMAT_RGBA, false, true);
	ICON_INJURY_SMOKE	= ResourceHandler::RegisterTexture2D("Icons/InjurySmoke.png", TEX_FORMAT_RGBA, false, true);
	ICON_SKILL_FIRE		= ResourceHandler::RegisterTexture2D("Icons/SkillFire.png", TEX_FORMAT_RGBA, false, true);
	ICON_SKILL_MEDIC	= ResourceHandler::RegisterTexture2D("Icons/SkillMedic.png", TEX_FORMAT_RGBA, false, true);
	ICON_SKILL_STRENGTH = ResourceHandler::RegisterTexture2D("Icons/SkillStrength.png", TEX_FORMAT_RGBA, false, true);
	ICON_CIRCLE			= ResourceHandler::RegisterTexture2D("Icons/Circle.png", TEX_FORMAT_RGBA, false, true);

	params.Wrap = TEX_PARAM_EDGECLAMP;
	HDR					= ResourceHandler::RegisterTexture2D("SkyBoxTextures/ocean.hdr", TEX_FORMAT_RGB16F, true, false, params);
}