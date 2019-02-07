#include <EnginePch.h>
#include <Graphics/Materials/CustomMaterial.h>

CustomMaterial::CustomMaterial()
	: m_pProgram(nullptr)
{
}

CustomMaterial::CustomMaterial(ShaderProgram* pProgram)
	: m_pProgram(pProgram)
{
}

CustomMaterial::~CustomMaterial()
{
	DeleteSafe(m_pProgram)
}