#include <EnginePch.h>
#include <IO/SHADER.h>
#include <IO/ResourceHandler.h>
#include <Graphics/Materials/WallMaterial.h>

#define TO_STRING(x) #x

uint32 SHADER::GUI = 0;
uint32 SHADER::FONT = 0;
uint32 SHADER::DEBUG_DECALS = 0;
uint32 SHADER::CBR_RESOLVE = 0;
uint32 SHADER::CBR_RECONSTRUCTION = 0;
uint32 SHADER::CBR_BLUR = 0;
uint32 SHADER::DEPTH_PRE_PASS = 0;
uint32 SHADER::FORWARD_PASS = 0;
uint32 SHADER::SKYBOX_PASS = 0;
uint32 SHADER::ORTHOGRAPHIC = 0;
uint32 SHADER::WALL_MATERIAL = 0;
uint32 SHADER::WATER_OUTDOOR_MATERIAL = 0;
uint32 SHADER::WATER_INDOOR_MATERIAL = 0;
uint32 SHADER::DEFERRED_DECALS = 0;
uint32 SHADER::STANDARD_MATERIAL = 0;
uint32 SHADER::EQUIREC_TO_CUBEMAP = 0;
uint32 SHADER::PARTICLES = 0;
uint32 SHADER::ANIMATION = 0;
uint32 SHADER::ANIMATION_DEPTH_PRE_PASS = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void SHADER::RegisterResourcesPreLoading()
{
	GUI						= ResourceHandler::RegisterShader("VShaderGUI.glsl", "FShaderGUI.glsl");
	FONT					= ResourceHandler::RegisterShader("VShaderFont.glsl", "FShaderFont.glsl");
}

void SHADER::RegisterResources()
{
	DEBUG_DECALS			= ResourceHandler::RegisterShader("debugVert.glsl", "debugDecalsFrag.glsl");
	CBR_RESOLVE				= ResourceHandler::RegisterShader("fullscreenTriVert.glsl", "cbrResolveFrag.glsl");
	CBR_RECONSTRUCTION		= ResourceHandler::RegisterShader("fullscreenTriVert.glsl", "cbrReconstructionFrag.glsl");
	CBR_BLUR				= ResourceHandler::RegisterShader("fullscreenTriVert.glsl", "cbrFilterFrag.glsl");
	DEPTH_PRE_PASS			= ResourceHandler::RegisterShader("depthPrePass.glsl", "depthPrePass.glsl");
	FORWARD_PASS			= ResourceHandler::RegisterShader("forwardVert.glsl", "forwardFrag.glsl");
	ORTHOGRAPHIC			= ResourceHandler::RegisterShader("orthoVert.glsl", "orthoFrag.glsl");
	EQUIREC_TO_CUBEMAP		= ResourceHandler::RegisterShader("VShaderEquirecToCubemap.glsl", "FShaderEquirecToCubemap.glsl");
	SKYBOX_PASS				= ResourceHandler::RegisterShader("skybox.glsl", "skybox.glsl");
#if defined(DEFERRED_RENDER_PATH)
	DEFERRED_DECALS			= ResourceHandler::RegisterShader("deferredDecals.glsl", "deferredDecals.glsl");
	STANDARD_MATERIAL		= ResourceHandler::RegisterShader("deferredMaterial.glsl", "deferredMaterial.glsl");
	WATER_MATERIAL			= ResourceHandler::RegisterShader("deferredWater.glsl", "deferredWater.glsl");
	PARTICLES				= ResourceHandler::RegisterShader("deferredParticles.glsl", "deferredParticles.glsl");
#elif defined(FORWARD_RENDER_PATH)
	DEFERRED_DECALS			= ResourceHandler::RegisterShader("deferredDecals.glsl", "deferredDecals.glsl");
	STANDARD_MATERIAL		= ResourceHandler::RegisterShader("forwardMaterial.glsl", "forwardMaterial.glsl");
	WATER_OUTDOOR_MATERIAL	= ResourceHandler::RegisterShader("forwardOutdoorWater.glsl", "forwardOutdoorWater.glsl");
	WATER_INDOOR_MATERIAL	= ResourceHandler::RegisterShader("forwardIndoorWater.glsl", "forwardIndoorWater.glsl");
	PARTICLES				= ResourceHandler::RegisterShader("forwardParticles.glsl", "forwardParticles.glsl");
	ANIMATION				= ResourceHandler::RegisterShader("forwardAnimation.glsl", "forwardAnimation.glsl");
	ANIMATION_DEPTH_PRE_PASS = ResourceHandler::RegisterShader("animatedDepthPrePass.glsl", "animatedDepthPrePass.glsl");
#endif

	{
		std::string str = (TO_STRING(WALL_STUMP_FROM_CENTER)) + std::string(" ") + std::to_string(WALL_STUMP_FROM_CENTER);
		const char* pDefines[] =
		{
			str.c_str()
		};

		ShaderDefines defines = {};
		defines.ppDefines = pDefines;
		defines.NumDefines = _countof(pDefines);

#if defined(DEFERRED_RENDER_PATH)
		WALL_MATERIAL		= ResourceHandler::RegisterShader("deferredWall.glsl", "deferredWall.glsl", defines);
#elif defined(FORWARD_RENDER_PATH)
		WALL_MATERIAL		= ResourceHandler::RegisterShader("forwardWall.glsl", "forwardWall.glsl", defines);
#endif
	}
}