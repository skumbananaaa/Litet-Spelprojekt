#include <EnginePch.h>
#include <IO/SHADER.h>
#include <IO/ResourceHandler.h>
#include <Graphics/Materials/WallMaterial.h>

#define TO_STRING(x) #x

uint32 SHADER::GUI = 0;
uint32 SHADER::FONT = 0;
uint32 SHADER::DEPTH_PRE_PASS = 0;
uint32 SHADER::FORWARD_PASS = 0;
uint32 SHADER::SKYBOX_PASS = 0;
uint32 SHADER::ORTHOGRAPHIC = 0;
uint32 SHADER::WALL_MATERIAL = 0;
uint32 SHADER::FLOOR = 0;
uint32 SHADER::WATER_OUTDOOR_MATERIAL = 0;
uint32 SHADER::WATER_INDOOR_MATERIAL = 0;
uint32 SHADER::STANDARD_MATERIAL = 0;
uint32 SHADER::EQUIREC_TO_CUBEMAP = 0;
uint32 SHADER::PARTICLES = 0;
uint32 SHADER::ANIMATION = 0;
uint32 SHADER::ANIMATION_DEPTH_PRE_PASS = 0;
uint32 SHADER::MESH_PARTICLES = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void SHADER::RegisterResourcesPreLoading()
{
	GUI							= ResourceHandler::RegisterShader("VShaderGUI.glsl", "FShaderGUI.glsl");
	FONT						= ResourceHandler::RegisterShader("VShaderFont.glsl", "FShaderFont.glsl");
	EQUIREC_TO_CUBEMAP			= ResourceHandler::RegisterShader("VShaderEquirecToCubemap.glsl", "FShaderEquirecToCubemap.glsl");
	SKYBOX_PASS					= ResourceHandler::RegisterShader("skybox.glsl", "skybox.glsl");
	DEPTH_PRE_PASS				= ResourceHandler::RegisterShader("depthPrePass.glsl", "depthPrePass.glsl");
	ANIMATION_DEPTH_PRE_PASS	= ResourceHandler::RegisterShader("animatedDepthPrePass.glsl", "animatedDepthPrePass.glsl");
	PARTICLES					= ResourceHandler::RegisterShader("forwardParticles.glsl", "forwardParticles.glsl");
	MESH_PARTICLES				= ResourceHandler::RegisterShader("forwardMeshParticles.glsl", "forwardMeshParticles.glsl");
	
	{
		//Strings for defines
		std::string numDirLights = (TO_STRING(NUM_DIRECTIONAL_LIGHTS)) + std::string(" ") + std::to_string(NUM_DIRECTIONAL_LIGHTS);
		std::string numSpotlights = (TO_STRING(NUM_SPOT_LIGHTS)) + std::string(" ") + std::to_string(NUM_SPOT_LIGHTS);
		std::string numPointLights = (TO_STRING(NUM_POINT_LIGHTS)) + std::string(" ") + std::to_string(NUM_POINT_LIGHTS);
		std::string levelY = (TO_STRING(LEVEL_SIZE_Y)) + std::string(" ") + std::to_string(LEVEL_SIZE_Y);
		std::string levelX = (TO_STRING(LEVEL_SIZE_X)) + std::string(" ") + std::to_string(LEVEL_SIZE_X);
		std::string levelZ = (TO_STRING(LEVEL_SIZE_Z)) + std::string(" ") + std::to_string(LEVEL_SIZE_Z);

		const char* pDefines[] =
		{
			numDirLights.c_str(),
			numPointLights.c_str(),
			numSpotlights.c_str(),
			levelX.c_str(),
			levelY.c_str(),
			levelZ.c_str(),
		};

		ShaderDefines defines = {};
		defines.ppDefines = pDefines;
		defines.NumDefines = _countof(pDefines);

		WATER_OUTDOOR_MATERIAL		= ResourceHandler::RegisterShader("forwardOutdoorWater.glsl", "forwardOutdoorWater.glsl", defines);
	}
}

void SHADER::RegisterResources()
{
	std::string numDirLights = (TO_STRING(NUM_DIRECTIONAL_LIGHTS)) + std::string(" ") + std::to_string(NUM_DIRECTIONAL_LIGHTS);
	std::string numSpotlights = (TO_STRING(NUM_SPOT_LIGHTS)) + std::string(" ") + std::to_string(NUM_SPOT_LIGHTS);
	std::string numPointLights = (TO_STRING(NUM_POINT_LIGHTS)) + std::string(" ") + std::to_string(NUM_POINT_LIGHTS);
	std::string levelY = (TO_STRING(LEVEL_SIZE_Y)) + std::string(" ") + std::to_string(LEVEL_SIZE_Y);
	std::string levelX = (TO_STRING(LEVEL_SIZE_X)) + std::string(" ") + std::to_string(LEVEL_SIZE_X);
	std::string levelZ = (TO_STRING(LEVEL_SIZE_Z)) + std::string(" ") + std::to_string(LEVEL_SIZE_Z);

	ORTHOGRAPHIC			= ResourceHandler::RegisterShader("orthoVert.glsl", "orthoFrag.glsl");
#if defined(DEFERRED_RENDER_PATH)
	DEFERRED_DECALS			= ResourceHandler::RegisterShader("deferredDecals.glsl", "deferredDecals.glsl");
	STANDARD_MATERIAL		= ResourceHandler::RegisterShader("deferredMaterial.glsl", "deferredMaterial.glsl");
	WATER_MATERIAL			= ResourceHandler::RegisterShader("deferredWater.glsl", "deferredWater.glsl");
	PARTICLES				= ResourceHandler::RegisterShader("deferredParticles.glsl", "deferredParticles.glsl");
#elif defined(FORWARD_RENDER_PATH)	
	{
		const char* pDefines[] =
		{
			numDirLights.c_str(),
			numPointLights.c_str(),
			numSpotlights.c_str(),
			levelX.c_str(),
			levelY.c_str(),
			levelZ.c_str(),
		};

		ShaderDefines defines = {};
		defines.ppDefines = pDefines;
		defines.NumDefines = _countof(pDefines);

		STANDARD_MATERIAL = ResourceHandler::RegisterShader("forwardMaterial.glsl", "forwardMaterial.glsl", defines);
		WATER_INDOOR_MATERIAL = ResourceHandler::RegisterShader("forwardIndoorWater.glsl", "forwardIndoorWater.glsl", defines);
	}

	{
		std::string maxBones = (TO_STRING(MAX_NUM_BONES)) + std::string(" ") + std::to_string(MAX_NUM_BONES);
		const char* pDefines[] =
		{
			maxBones.c_str(),
			numDirLights.c_str(),
			numPointLights.c_str(),
			numSpotlights.c_str(),
			levelX.c_str(),
			levelY.c_str(),
			levelZ.c_str(),
		};

		ShaderDefines defines = {};
		defines.ppDefines = pDefines;
		defines.NumDefines = _countof(pDefines);

		ANIMATION = ResourceHandler::RegisterShader("forwardAnimation.glsl", "forwardAnimation.glsl", defines);
	}
#endif

	{
		std::string str = (TO_STRING(WALL_STUMP_FROM_CENTER)) + std::string(" ") + std::to_string(WALL_STUMP_FROM_CENTER);
		const char* pDefines[] =
		{
			str.c_str(),
			numDirLights.c_str(),
			numPointLights.c_str(),
			numSpotlights.c_str(),
			levelX.c_str(),
			levelY.c_str(),
			levelZ.c_str(),
		};

		ShaderDefines defines = {};
		defines.ppDefines = pDefines;
		defines.NumDefines = _countof(pDefines);

#if defined(DEFERRED_RENDER_PATH)
		WALL_MATERIAL		= ResourceHandler::RegisterShader("deferredWall.glsl", "deferredWall.glsl", defines);
#elif defined(FORWARD_RENDER_PATH)
		WALL_MATERIAL		= ResourceHandler::RegisterShader("forwardWall.glsl", "forwardWall.glsl", defines);
		FLOOR				= ResourceHandler::RegisterShader("forwardFloor.glsl", "forwardFloor.glsl", defines);
#endif
	}
}