#include <EnginePch.h>
#include <IO/MATERIAL.h>
#include <IO/ResourceHandler.h>

uint32 MATERIAL::BLACK = 0;
uint32 MATERIAL::WHITE = 0;
uint32 MATERIAL::RED = 0;
uint32 MATERIAL::RED_1 = 0;
uint32 MATERIAL::RED_2 = 0;
uint32 MATERIAL::RED_3 = 0;
uint32 MATERIAL::RED_4 = 0;
uint32 MATERIAL::GREEN = 0;
uint32 MATERIAL::BLUE = 0;
uint32 MATERIAL::BOAT = 0;
uint32 MATERIAL::GROUND = 0;
uint32 MATERIAL::WATER = 0;
uint32 MATERIAL::WALL_STANDARD = 0;
uint32 MATERIAL::CREW_STANDARD = 0;

void MATERIAL::RegisterResources()
{
	Shader defferedVS;
	if (defferedVS.CompileFromFile("Resources/Shaders/deferredMaterial.glsl", VERTEX_SHADER))
	{
		std::cout << "Created Geometrypass Vertex shader" << std::endl;
	}

	Shader defferedFS;
	if (defferedFS.CompileFromFile("Resources/Shaders/deferredMaterial.glsl", FRAGMENT_SHADER))
	{
		std::cout << "Created Geometrypass Fragment shader" << std::endl;
	}

	ShaderProgram* pProgram = new ShaderProgram(defferedVS, defferedFS);

	BLACK				= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)		, 256.0f, -1, pProgram);
	WHITE				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)		, 256.0f, -1, pProgram);
	RED					= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 256.0f, -1, pProgram);
	RED_1				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 128.0f, -1, pProgram);
	RED_2				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 96.0f, -1, pProgram);
	RED_3				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 32.0f, -1, pProgram);
	RED_4				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 16.0f, -1, pProgram);
	GREEN				= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)		, 256.0f, -1, pProgram);
	BLUE				= ResourceHandler::RegisterMaterial(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)		, 256.0f, -1, pProgram);
	BOAT				= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)		, 256.0f, -1, pProgram);
	GROUND				= ResourceHandler::RegisterMaterial(glm::vec4(0.471f, 0.282f, 0.11f, 1.0f)	, 256.0f, -1, pProgram);
	CREW_STANDARD		= ResourceHandler::RegisterMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)		, 256.0f, -1, pProgram);
	WALL_STANDARD		= ResourceHandler::RegisterWallMaterial(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)		, 256.0f, -1);
	WATER				= ResourceHandler::RegisterWaterMaterial(TEXTURE::WATER_DISTORTION, TEXTURE::WATER_NORMAL);
}