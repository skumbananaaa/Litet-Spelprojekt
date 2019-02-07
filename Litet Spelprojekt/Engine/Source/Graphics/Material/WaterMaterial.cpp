#include <EnginePch.h>
#include <Graphics/Materials/WaterMaterial.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>

WaterMaterial::WaterMaterial()
	: m_pReflector(nullptr),
	m_pDistortion(nullptr),
	m_pNormalMap(nullptr)
{
	Shader vs;
	vs.CompileFromFile("Resources/Shaders/VShaderWater.glsl", VERTEX_SHADER);

	Shader fs;
	fs.CompileFromFile("Resources/Shaders/FShaderWater.glsl", FRAGMENT_SHADER);

	SetProgram(new ShaderProgram(vs, fs));
}

WaterMaterial::~WaterMaterial()
{
}

void WaterMaterial::Bind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetProgram();

	context.SetTexture(m_pReflector->GetReflectionTexture(), 0);
	context.SetTexture(m_pDistortion, 1);
	context.SetTexture(m_pNormalMap, 2);
}

void WaterMaterial::Unbind() const noexcept
{
}

void WaterMaterial::SetPlanarReflector(PlanarReflector* pReflector)
{
}

void WaterMaterial::SetDistortionTexture(Texture2D* pDistortion)
{
}

void WaterMaterial::SetNormalMap(Texture2D* pNormalMap)
{
}