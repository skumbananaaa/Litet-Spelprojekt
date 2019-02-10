#include <EnginePch.h>
#include <IO/SHADER.h>
#include <IO/ResourceHandler.h>

uint32 SHADER::GUI;

/*
* Used for preloading resources needed in the loading screen
*/
void SHADER::RegisterResourcesPreLoading()
{
	GUI					= ResourceHandler::RegisterShader("VShaderGUI.glsl", "FShaderGUI.glsl");
}

void SHADER::RegisterResources()
{
	
}