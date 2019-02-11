#include <EnginePch.h>
#include <IO/SHADER.h>
#include <IO/ResourceHandler.h>

uint32 SHADER::GUI;
uint32 SHADER::FONT;


/*
* Used for preloading resources needed in the loading screen
*/
void SHADER::RegisterResourcesPreLoading()
{
	GUI					= ResourceHandler::RegisterShader("VShaderGUI.glsl", "FShaderGUI.glsl");
	FONT				= ResourceHandler::RegisterShader("VShaderFont.glsl", "FShaderFont.glsl");
}

void SHADER::RegisterResources()
{
	
}