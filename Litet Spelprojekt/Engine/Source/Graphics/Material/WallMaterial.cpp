#include <EnginePch.h>
#include <Graphics/Materials/WallMaterial.h>

#define TO_STRING(x) #x

WallMaterial::WallMaterial()
	: m_pDissolveBuffer(nullptr)
{
	std::string str = (TO_STRING(WALL_STUMP_FROM_CENTER)) + std::string(" ") + std::to_string(WALL_STUMP_FROM_CENTER);
	const char* pDefines[] = 
	{
		str.c_str()
	};

	ShaderDefines defines = {};
	defines.ppDefines = pDefines;
	defines.NumDefines = _countof(pDefines);

	Shader vs;
	vs.CompileFromFile("Resources/Shaders/deferredWall.glsl", VERTEX_SHADER, defines);

	Shader fs;
	fs.CompileFromFile("Resources/Shaders/deferredWall.glsl", FRAGMENT_SHADER, defines);

	SetProgram(new ShaderProgram(vs, fs));

	m_Buffer.DissolveFactor = 0.0f;
	m_pDissolveBuffer = new UniformBuffer(&m_Buffer, 1, sizeof(DissolveBuffer));
}

WallMaterial::~WallMaterial()
{
	DeleteSafe(m_pDissolveBuffer);
}

void WallMaterial::Bind(const Framebuffer* pGBuffer) const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.Enable(CLIP_DISTANCE0);
	context.Enable(CLIP_DISTANCE1);
	context.Enable(CLIP_DISTANCE2);
	context.Disable(CULL_FACE);

	context.SetUniformBuffer(m_pDissolveBuffer, 3);

	Material::Bind(pGBuffer);
}

void WallMaterial::Unbind() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.Enable(CULL_FACE);
	context.Disable(CLIP_DISTANCE0);
	context.Disable(CLIP_DISTANCE1);
	context.Disable(CLIP_DISTANCE2);

	context.SetUniformBuffer(nullptr, 3);

	Material::Unbind();
}

void WallMaterial::SetDissolveFactor(float dissolveFactor) const noexcept
{
	m_Buffer.DissolveFactor = dissolveFactor;
	m_pDissolveBuffer->UpdateData(&m_Buffer);
}

void WallMaterial::SetClipPlane(const glm::vec4& plane, uint32 index) const noexcept
{
	m_Buffer.g_ClipDistances[index] = plane;
	m_pDissolveBuffer->UpdateData(&m_Buffer);
}
