#include <EnginePch.h>
#include <Graphics/Materials/WaterMaterial.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>

WaterMaterial::WaterMaterial()
	: m_pReflector(nullptr),
	m_pDistortion(nullptr),
	m_pDepthMap(nullptr),
	m_pWaterBuffer(nullptr),
	m_Buffer()
{
	Shader vs;
	vs.CompileFromFile("Resources/Shaders/waterVert.glsl", VERTEX_SHADER);

	Shader fs;
	fs.CompileFromFile("Resources/Shaders/waterFrag.glsl", FRAGMENT_SHADER);

	SetProgram(new ShaderProgram(vs, fs));

	m_Buffer.DistortionFactor = 0.0f;
	m_pWaterBuffer = new UniformBuffer(&m_Buffer, 1, sizeof(WaterBuffer));
}

WaterMaterial::~WaterMaterial()
{
	DeleteSafe(m_pWaterBuffer);
}

void WaterMaterial::Bind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	//context.SetProgram(GetProgram());

	if (m_pReflector)
	{
		context.SetTexture(m_pReflector->GetReflectionTexture(), 4);
	}

	context.SetTexture(m_pDistortion, 3);
	//context.SetTexture(m_pDepthMap, 3);

	context.SetUniformBuffer(m_pWaterBuffer, 3);

	Material::Bind();
}

void WaterMaterial::Unbind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetTexture(nullptr, 0);
	context.SetTexture(nullptr, 1);
	context.SetTexture(nullptr, 2);
	context.SetTexture(nullptr, 3);

	Material::Unbind();
}

void WaterMaterial::SetPlanarReflector(PlanarReflector* pReflector) const
{
	m_pReflector = pReflector;
}

void WaterMaterial::SetDistortionTexture(Texture2D* pDistortion)
{
	m_pDistortion = pDistortion;
}

void WaterMaterial::SetDistortionFactor(float distortionFactor) const
{
	m_Buffer.DistortionFactor = distortionFactor;
	m_pWaterBuffer->UpdateData(&m_Buffer);
}
