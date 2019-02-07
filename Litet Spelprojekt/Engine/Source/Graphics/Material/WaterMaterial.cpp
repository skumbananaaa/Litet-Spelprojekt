#include <EnginePch.h>
#include <Graphics/Materials/WaterMaterial.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>

WaterMaterial::WaterMaterial()
	: m_pReflector(nullptr),
	m_pDistortion(nullptr),
	m_pNormalMap(nullptr),
	m_pDepthMap(nullptr)
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

	context.SetProgram(GetProgram());

	context.SetTexture(m_pReflector->GetReflectionTexture(), 0);
	context.SetTexture(m_pDistortion, 1);
	context.SetTexture(m_pNormalMap, 2);
	context.SetTexture(m_pDepthMap, 3);
}

void WaterMaterial::Unbind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetTexture(nullptr, 0);
	context.SetTexture(nullptr, 1);
	context.SetTexture(nullptr, 2);
	context.SetTexture(nullptr, 3);
}

void WaterMaterial::SetPlanarReflector(PlanarReflector* pReflector)
{
	m_pReflector = pReflector;
}

void WaterMaterial::SetDistortionTexture(Texture2D* pDistortion)
{
	m_pDistortion = pDistortion;
}

void WaterMaterial::SetNormalMap(Texture2D* pNormalMap)
{
	m_pNormalMap = pNormalMap;
}