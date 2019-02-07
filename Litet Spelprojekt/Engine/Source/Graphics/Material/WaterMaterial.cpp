#include <EnginePch.h>
#include <Graphics/Materials/WaterMaterial.h>
#include <Graphics/Renderers/GLContext.h>

WaterMaterial::WaterMaterial()
	: m_pReflector(nullptr),
	m_pDistortion(nullptr),
	m_pNormalMap(nullptr)
{
}

WaterMaterial::~WaterMaterial()
{
}

void WaterMaterial::Bind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	
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