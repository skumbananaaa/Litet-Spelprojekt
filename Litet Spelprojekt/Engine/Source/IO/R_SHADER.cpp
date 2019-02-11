#include <EnginePch.h>
#include <IO/SHADER.h>
#include <IO/ResourceHandler.h>

#define TO_STRING(x) #x

uint32 SHADER::GUI = 0;
uint32 SHADER::FONT = 0;
uint32 SHADER::DEBUG_DECALS = 0;
uint32 SHADER::DEFERRED_WALL = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void SHADER::RegisterResourcesPreLoading()
{
	GUI					= ResourceHandler::RegisterShader("VShaderGUI.glsl", "FShaderGUI.glsl");
	FONT				= ResourceHandler::RegisterShader("VShaderFont.glsl", "FShaderFont.glsl");
	DEBUG_DECALS		= ResourceHandler::RegisterShader("debugVert.glsl", "debugDecalsFrag.glsl");

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

void SHADER::RegisterResources()
{
	
}