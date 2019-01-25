#include <EnginePch.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <System/Window.h>
#include <System/Application.h>

#define REFLECTIONSIZE 512

DefferedRenderer::DefferedRenderer()
	: m_pGBuffer(nullptr),
	m_pDepthPrePassProgram(nullptr),
	m_pGeometryPassProgram(nullptr),
	m_pLightPassProgram(nullptr),
	m_pGPassVSPerFrame(nullptr),
	m_pGPassVSPerObject(nullptr),
	m_pGPassFSPerObject(nullptr),
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

	if (m_pDepthPrePassProgram != nullptr)
	{
		delete m_pDepthPrePassProgram;
		m_pDepthPrePassProgram = nullptr;
	}

	if (m_pGPassFSPerObject != nullptr)
	{
		delete m_pGPassFSPerObject;
		m_pGPassFSPerObject = nullptr;
	}
}

void DefferedRenderer::DrawScene(const Scene& scene, float dtS) const
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	context.SetClearDepth(1.0f);

	//DepthPrePass(scene);

	GeometryPass(scene.GetGameObjects(), scene.GetCamera(), m_pGBuffer);
	LightPass(scene.GetCamera(), nullptr, m_pGBuffer);

	context.BlitFramebuffer(nullptr, m_pGBuffer, CLEAR_FLAG_DEPTH);

	WaterPass(scene, dtS);

	context.Enable(DEPTH_TEST);
	context.SetDepthMask(true);
	context.SetDepthFunc(FUNC_LESS);
	context.SetFramebuffer(nullptr);
}

void DefferedRenderer::Create() noexcept
{
	std::cout << "Createing deffered renderer" << std::endl;

	//We can destroy desc when gbuffer is created
	{
		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.ColorAttchmentFormats[1] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 2;
		desc.DepthStencilFormat = TEX_FORMAT_DEPTH;
		//desc.Width = 1920; 
		desc.Width = Window::GetCurrentWindow().GetWidth();
		//desc.Height = 1080;
		desc.Height = Window::GetCurrentWindow().GetHeight();

		m_pGBuffer = new Framebuffer(desc);
	}

	{
		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.ColorAttchmentFormats[1] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 2;
		desc.DepthStencilFormat = TEX_FORMAT_DEPTH;
		desc.Width = REFLECTIONSIZE;
		desc.Height = REFLECTIONSIZE;

		m_pWaterGBuffer = new Framebuffer(desc);
	}


	{
		TextureParams params = {};
		params.Wrap = TEX_PARAM_REPEAT;
		params.MinFilter = TEX_LINEAR;
		params.MagFilter = TEX_LINEAR;

		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 1;
		desc.SamplingParams = params;
		desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
		desc.Width = REFLECTIONSIZE;
		desc.Height = REFLECTIONSIZE;

		m_pReflection = new Framebuffer(desc);
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedGeometryVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Geomtrypass Vertex shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if(pFrag->CompileFromFile("Resources/Shaders/defferedGeometryFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Geomtrypass Fragment shader" << std::endl;
		}

		m_pGeometryPassProgram = new ShaderProgram(*pVert, *pFrag);

		delete pVert;
		delete pFrag;
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedLightningVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Lightpass Vertex shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/defferedLightningFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Lightpass Fragment shader" << std::endl;
		}

		m_pLightPassProgram = new ShaderProgram(*pVert, *pFrag);

		delete pVert;
		delete pFrag;
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedDepthPreVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created DepthPrePass Vertex shader" << std::endl;
		}

		m_pDepthPrePassProgram = new ShaderProgram(*pVert);

		delete pVert;
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/VShaderWater.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Water Vertex shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/FShaderWater.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Water Fragment shader" << std::endl;
		}

		m_pWaterpassProgram = new ShaderProgram(*pVert, *pFrag);

		delete pVert;
		delete pFrag;
	}

	//We can destroy object when uniformbuffer is created
	{
		GPassVSPerFrame object = {};
		object.ViewProjection = glm::mat4(1.0f);
		object.CameraPosition = glm::vec3();

		m_pGPassVSPerFrame = new UniformBuffer(&object, 1, sizeof(GPassVSPerFrame));
	}

	{
		GPassVSPerObject object = {};
		object.Model = glm::mat4(1.0f);

		m_pGPassVSPerObject = new UniformBuffer(&object, 1, sizeof(GPassVSPerObject));
	}

	{
		GPassFSPerObject object = {};
		object.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		m_pGPassFSPerObject = new UniformBuffer(&object, 1, sizeof(GPassFSPerObject));
	}

	{
		WaterPassPerFrame object = {};
		object.CameraCombined = glm::mat4(1.0f);
		object.CameraPosition = glm::vec3();
		object.DistortionMoveFactor = 0.0f;

		m_pWaterPassPerFrame = new UniformBuffer(&object, 1, sizeof(WaterPassPerFrame));
	}

	{
		WaterPassPerObjectVS object = {};
		object.Model = glm::mat4(1.0f);

		m_pWaterPassPerObject = new UniformBuffer(&object, 1, sizeof(WaterPassPerObjectVS));
	}

	{
		LightPassBuffer buff = {};
		buff.InverseView = glm::mat4(1.0f);
		buff.InverseProjection = glm::mat4(1.0f);
		buff.CameraPosition = glm::vec3();

		m_pLightPassBuffer = new UniformBuffer(&buff, 1, sizeof(LightPassBuffer));
	}

	{
		m_pTriangle = new FullscreenTri();
	}

	{
		TextureParams params = {};
		params.Wrap = TEX_PARAM_REPEAT;
		params.MinFilter = TEX_LINEAR;
		params.MagFilter = TEX_LINEAR;

		m_pWaterDistortionMap = new Texture2D("Resources/Textures/waterDUDV.png", TEX_FORMAT::TEX_FORMAT_RGB, false, params);
		m_pWaterNormalMap = new Texture2D("Resources/Textures/waterNormalMap.png", TEX_FORMAT::TEX_FORMAT_RGB, false, params);
	}
}

void DefferedRenderer::DepthPrePass(const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(m_pDepthPrePassProgram);

	context.SetColorMask(0, 0, 0, 0);
	context.SetDepthFunc(FUNC_LESS);

	GPassVSPerFrame perFrame = {};
	perFrame.ViewProjection = scene.GetCamera().GetCombinedMatrix();
	perFrame.CameraPosition = scene.GetCamera().GetPosition();
	m_pGPassVSPerFrame->UpdateData(&perFrame);

	GPassVSPerObject perObject = {};
	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		perObject.Model = scene.GetGameObjects()[i]->GetTransform();
		m_pGPassVSPerObject->UpdateData(&perObject);

		context.DrawIndexedMesh(scene.GetGameObjects()[i]->GetMesh());
	}

	context.SetDepthFunc(FUNC_LESS_EQUAL);
	context.SetColorMask(1, 1, 1, 1);

	context.SetDepthMask(false);
}

void DefferedRenderer::GeometryPass(const std::vector<GameObject*>& gameobjects, const Camera& camera, const Framebuffer* const pFramebuffer) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetViewport(pFramebuffer->GetWidth(), pFramebuffer->GetHeight(), 0, 0);
	context.SetFramebuffer(pFramebuffer);
	context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

	context.SetProgram(m_pGeometryPassProgram);

	context.SetUniformBuffer(m_pGPassVSPerFrame, 0);
	context.SetUniformBuffer(m_pGPassVSPerObject, 1);
	context.SetUniformBuffer(m_pGPassFSPerObject, 2);

	GPassVSPerFrame perFrame = {};
	perFrame.ViewProjection = camera.GetCombinedMatrix();
	perFrame.CameraPosition = camera.GetPosition();
	m_pGPassVSPerFrame->UpdateData(&perFrame);

	GPassVSPerObject perObjectVS = {};
	GPassFSPerObject perObjectFS = {};

	for (uint32 i = 0; i < gameobjects.size(); i++)
	{
		GameObject& gameobject = *gameobjects[i];
		if (gameobject.HasMaterial())
		{
			perObjectVS.Model = gameobject.GetTransform();
			m_pGPassVSPerObject->UpdateData(&perObjectVS);

			const Material& material = gameobject.GetMaterial();
			perObjectFS.Color = material.GetColor();
			if (material.HasTexture())
			{
				perObjectFS.HasTexture = 1.0f;
				context.SetTexture(material.GetTexture(), 0);
			}
			else
			{
				perObjectFS.HasTexture = 0.0f;
			}

			if (material.HasNormalMap())
			{
				perObjectFS.HasNormalMap = 1.0f;
				context.SetTexture(material.GetNormalMap(), 1);
			}
			else
			{
				perObjectFS.HasNormalMap = 0.0f;
			}

			m_pGPassFSPerObject->UpdateData(&perObjectFS);

			context.DrawIndexedMesh(gameobject.GetMesh());
		}
	}
}

void DefferedRenderer::LightPass(const Camera& camera, const Framebuffer* const pFramebuffer, const Framebuffer* const pGBuffer) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetFramebuffer(pFramebuffer);
	context.Clear(CLEAR_FLAG_COLOR);

	context.Disable(DEPTH_TEST);

	if (pGBuffer != nullptr)
	{
		context.SetViewport(pGBuffer->GetWidth(), pGBuffer->GetHeight(), 0, 0);
	}
	else
	{
		context.SetViewport(Window::GetCurrentWindow().GetWidth(), Window::GetCurrentWindow().GetHeight(), 0, 0);
	}

	context.SetProgram(m_pLightPassProgram);
	context.SetUniformBuffer(m_pLightPassBuffer, 0);

	{
		LightPassBuffer buff = {};
		buff.InverseView = glm::inverse(camera.GetViewMatrix());
		buff.InverseProjection = glm::inverse(camera.GetProjectionMatrix());
		buff.CameraPosition = camera.GetPosition();

		m_pLightPassBuffer->UpdateData(&buff);
	}

	context.SetTexture(pGBuffer->GetColorAttachment(0), 0);
	context.SetTexture(pGBuffer->GetColorAttachment(1), 1);

	context.DrawFullscreenTriangle(*m_pTriangle);
}

void DefferedRenderer::WaterPass(const Scene& scene, float dtS) const noexcept
{
	static float dist = 0.0f;

	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetUniformBuffer(m_pGPassFSPerObject, 2);

	Camera reflectionCam = scene.GetCamera();
	float reflDistance = reflectionCam.GetPosition().y * 2.0f;
	reflectionCam.SetPos(reflectionCam.GetPosition() - glm::vec3(0.0f, reflDistance, 0.0f));
	reflectionCam.InvertPitch();
	reflectionCam.UpdateFromPitchYaw();

	context.Enable(Cap::CLIP_DISTANCE0);
	GeometryPass(scene.GetGameObjects(), reflectionCam, m_pWaterGBuffer);
	context.Disable(Cap::CLIP_DISTANCE0);
	
	LightPass(reflectionCam, m_pReflection, m_pWaterGBuffer);

	//Start rendering forward
	context.SetProgram(m_pWaterpassProgram);
	context.Enable(DEPTH_TEST);

	context.SetFramebuffer(nullptr);
	context.SetViewport(Window::GetCurrentWindow().GetWidth(), Window::GetCurrentWindow().GetHeight(), 0, 0);
	
	context.SetTexture(m_pReflection->GetColorAttachment(0), 0);
	context.SetTexture(m_pWaterDistortionMap, 1);
	context.SetTexture(m_pWaterNormalMap, 2);
	context.SetTexture(m_pGBuffer->GetDepthAttachment(), 3);

	WaterPassPerFrame perFrame = {};
	perFrame.CameraCombined = scene.GetCamera().GetCombinedMatrix();
	perFrame.CameraPosition = scene.GetCamera().GetPosition();
	
	dist += 0.02f * dtS;
	perFrame.DistortionMoveFactor = dist;

	m_pWaterPassPerFrame->UpdateData(&perFrame);

	context.SetUniformBuffer(m_pWaterPassPerObject, 0);
	context.SetUniformBuffer(m_pWaterPassPerFrame, 1);

	WaterPassPerObjectVS perObject= {};
	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		GameObject& gameobject = *scene.GetGameObjects()[i];
		if (!gameobject.HasMaterial())
		{
			perObject.Model = gameobject.GetTransform();
			m_pWaterPassPerObject->UpdateData(&perObject);

			context.DrawIndexedMesh(gameobject.GetMesh());
		}
	}
}
