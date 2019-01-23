#include <EnginePch.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <System/Window.h>
#include <System/Application.h>

DefferedRenderer::DefferedRenderer()
	: m_pGBuffer(nullptr),
	m_pGeometryPassProgram(nullptr),
	m_pLightPassProgram(nullptr),
	m_pGPassVSPerFrame(nullptr),
	m_pGPassVSPerObject(nullptr),
	m_pLightPassBuffer(nullptr),
	m_pTriangle(nullptr)
{
	Create();
}

DefferedRenderer::~DefferedRenderer()
{
	if (m_pGBuffer != nullptr)
	{
		delete m_pGBuffer;
		m_pGBuffer = nullptr;
	}

	if (m_pGeometryPassProgram != nullptr)
	{
		delete m_pGeometryPassProgram;
		m_pGeometryPassProgram = nullptr;
	}

	if (m_pLightPassProgram != nullptr)
	{
		delete m_pLightPassProgram;
		m_pLightPassProgram = nullptr;
	}

	if (m_pGPassVSPerFrame != nullptr)
	{
		delete m_pGPassVSPerFrame;
		m_pGPassVSPerFrame = nullptr;
	}

	if (m_pGPassVSPerObject != nullptr)
	{
		delete m_pGPassVSPerObject;
		m_pGPassVSPerObject = nullptr;
	}

	if (m_pLightPassBuffer != nullptr)
	{
		delete m_pLightPassBuffer;
		m_pLightPassBuffer = nullptr;
	}

	if (m_pTriangle != nullptr)
	{
		delete m_pTriangle;
		m_pTriangle = nullptr;
	}
}

void DefferedRenderer::DrawScene(const Scene& scene) const
{
	GLContext& context = Application::GetInstance().GetContext();

	context.SetViewport(m_pGBuffer->GetWidth(), m_pGBuffer->GetHeight(), 0, 0);
	context.SetFramebuffer(m_pGBuffer);
	context.Clear(CLEAR_FLAG_DEPTH);

	GeometryPass(scene);

	context.SetFramebuffer(nullptr);
	context.Disable(DEPTH_TEST);

	LightPass(scene);

	context.Enable(DEPTH_TEST);
}

void DefferedRenderer::Create()
{
	std::cout << "Createing deffered renderer" << std::endl;

	//We can destroy desc when gbuffer is created
	{
		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.ColorAttchmentFormats[1] = TEX_FORMAT_RGBA16F;
		desc.ColorAttchmentFormats[2] = TEX_FORMAT_RGBA16F;
		desc.NumColorAttachments = 3;
		desc.DepthStencilFormat = TEX_FORMAT_DEPTH_STENCIL;
		desc.Width = 1920;
		desc.Height = 1080;

		m_pGBuffer = new Framebuffer(desc);
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedGeometryVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if(pFrag->CompileFromFile("Resources/Shaders/defferedGeometryFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created shader" << std::endl;
		}

		m_pGeometryPassProgram = new ShaderProgram(*pVert, *pFrag);

		delete pVert;
		delete pFrag;
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedLightningVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/defferedLightningFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created shader" << std::endl;
		}

		m_pLightPassProgram = new ShaderProgram(*pVert, *pFrag);

		delete pVert;
		delete pFrag;
	}

	//We can destroy object when uniformbuffer is created
	{
		GPassPerFrame object = {};
		object.ViewProjection = glm::mat4(1.0f);
		object.CameraPosition = glm::vec3();

		m_pGPassVSPerFrame = new UniformBuffer(&object, 1, sizeof(GPassPerFrame));
	}

	{
		GPassPerObject object = {};
		object.Model = glm::mat4(1.0f);

		m_pGPassVSPerObject = new UniformBuffer(&object, 1, sizeof(GPassPerFrame));
	}

	{
		LightPassBuffer buff = {};
		buff.CameraPosition = glm::vec3();

		m_pLightPassBuffer = new UniformBuffer(&buff, 1, sizeof(LightPassBuffer));
	}

	{
		m_pTriangle = new FullscreenTri();
	}
}

void DefferedRenderer::GeometryPass(const Scene& scene) const
{
	GLContext& context = Application::GetInstance().GetContext();

	context.SetClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	context.Clear(CLEAR_FLAG_COLOR);
	
	context.SetProgram(m_pGeometryPassProgram);

	context.SetUniformBuffer(m_pGPassVSPerFrame, 0);
	context.SetUniformBuffer(m_pGPassVSPerObject, 1);
	
	GPassPerFrame perFrame = {};
	perFrame.ViewProjection = scene.GetCamera().GetCombinedMatrix();
	perFrame.CameraPosition = scene.GetCamera().GetPosition();
	m_pGPassVSPerFrame->UpdateData(&perFrame);

	GPassPerObject perObject = {};
	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		perObject.Model = scene.GetGameObjects()[i]->GetTransform();
		m_pGPassVSPerObject->UpdateData(&perObject);

		context.DrawIndexedMesh(scene.GetGameObjects()[i]->GetMesh());
	}
}

void DefferedRenderer::LightPass(const Scene& scene) const
{
	GLContext& context = Application::GetInstance().GetContext();


	context.SetViewport(Window::GetCurrentWindow().GetWidth(), Window::GetCurrentWindow().GetHeight(), 0, 0);
	context.SetProgram(m_pLightPassProgram);
	context.SetUniformBuffer(m_pLightPassBuffer, 0);

	{
		LightPassBuffer buff = {};
		buff.CameraPosition = scene.GetCamera().GetPosition();

		m_pLightPassBuffer->UpdateData(&buff);
	}

	context.SetTexture(m_pGBuffer->GetColorAttachment(0), 0);
	context.SetTexture(m_pGBuffer->GetColorAttachment(1), 1);
	context.SetTexture(m_pGBuffer->GetColorAttachment(2), 2);

	context.DrawFullscreenTriangle(*m_pTriangle);
}
