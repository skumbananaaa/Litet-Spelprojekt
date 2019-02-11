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
uint32 SHADER::WATER_PASS = 0;
uint32 SHADER::DECAL_PASS = 0;
uint32 SHADER::FORWARD_PASS = 0;
uint32 SHADER::SKYBOX_PASS = 0;
uint32 SHADER::ORTHOGRAPHIC = 0;
uint32 SHADER::DEFERRED_WALL = 0;
uint32 SHADER::DEFERRED_WATER = 0;
uint32 SHADER::DEFERRED_MATERIAL = 0;
uint32 SHADER::EQUIREC_TO_CUBEMAP = 0;

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
	DEPTH_PRE_PASS			= ResourceHandler::RegisterShader("defferedDepthPreVert.glsl");
	WATER_PASS				= ResourceHandler::RegisterShader("VShaderWater.glsl", "FShaderWater.glsl");
	DECAL_PASS				= ResourceHandler::RegisterShader("defferedDecalsVert.glsl", "defferedDecalsFrag.glsl");
	FORWARD_PASS			= ResourceHandler::RegisterShader("forwardVert.glsl", "forwardFrag.glsl");
	SKYBOX_PASS				= ResourceHandler::RegisterShader("VShaderSkyBox.glsl", "FShaderSkyBox.glsl");
	ORTHOGRAPHIC			= ResourceHandler::RegisterShader("orthoVert.glsl", "orthoFrag.glsl");
	DEFERRED_MATERIAL		= ResourceHandler::RegisterShader("deferredMaterial.glsl", "deferredMaterial.glsl");
	DEFERRED_WATER			= ResourceHandler::RegisterShader("waterVert.glsl", "waterFrag.glsl");
	EQUIREC_TO_CUBEMAP		= ResourceHandler::RegisterShader("VShaderEquirecToCubemap.glsl", "FShaderEquirecToCubemap.glsl");

	{
		std::string str = (TO_STRING(WALL_STUMP_FROM_CENTER)) + std::string(" ") + std::to_string(WALL_STUMP_FROM_CENTER);
		const char* pDefines[] =
		{
			str.c_str()
		};

		ShaderDefines defines = {};
		defines.ppDefines = pDefines;
		defines.NumDefines = _countof(pDefines);
		DEFERRED_WALL		= ResourceHandler::RegisterShader("deferredWall.glsl", "deferredWall.glsl", defines);
	}
}