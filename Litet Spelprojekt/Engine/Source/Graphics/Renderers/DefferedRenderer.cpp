#include <EnginePch.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <System/Window.h>
#include <System/Application.h>

#define REFLECTIONSIZE 384

DefferedRenderer::DefferedRenderer()
	: m_pGBufferCBR(nullptr),
	m_pTriangle(nullptr),
	m_pDecalMesh(nullptr),
	m_pBlur(nullptr),
	m_pLightBuffer(nullptr),
	m_pForwardPass(nullptr),
	m_pCbrBlurProgram(nullptr),
	m_pCbrReconstructionProgram(nullptr),
	m_pCbrResolveProgram(nullptr),
	m_pDepthPrePassProgram(nullptr),
	m_pGeometryPassProgram(nullptr),
	m_pDecalsPassProgram(nullptr),
	m_pSkyBoxPassProgram(nullptr),
	m_pResolveTarget(nullptr)
{
	Create();
}

DefferedRenderer::~DefferedRenderer()
{
	DeleteSafe(m_pResolveTarget);
	DeleteSafe(m_pGBufferCBR);
	DeleteSafe(m_pBlur);

	DeleteSafe(m_pTriangle);
	
	DeleteSafe(m_pLightBuffer);
	DeleteSafe(m_pMaterialBuffer);
	DeleteSafe(m_pCameraBuffer);

	DeleteSafe(m_pSkyBoxPassPerFrame);
	DeleteSafe(m_pSkyBoxPassPerObject);
	
	DeleteSafe(m_pCbrBlurProgram);
	DeleteSafe(m_pCbrReconstructionProgram);
	DeleteSafe(m_pCbrResolveProgram);
	DeleteSafe(m_pDepthPrePassProgram);
	DeleteSafe(m_pGeometryPassProgram);
	DeleteSafe(m_pDecalsPassProgram);
	DeleteSafe(m_pForwardPass);
	DeleteSafe(m_pForwardPass);
	DeleteSafe(m_pSkyBoxPassProgram);
}

void DefferedRenderer::SetClipDistance(const glm::vec4& plane, uint32 index)
{
	assert(index < NUM_CLIP_DISTANCES);
	m_ClipDistances[index] = plane;
}

void DefferedRenderer::Create() noexcept
{
	std::cout << "Creating deffered renderer" << std::endl;

	//CREATE FRAMEBUFFERS
	{
		TextureParams params = {};
		params.MinFilter = TEX_PARAM_NEAREST;
		params.MagFilter = TEX_PARAM_NEAREST;
		params.Wrap = TEX_PARAM_EDGECLAMP;

		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.ColorAttchmentFormats[1] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 2;
		desc.DepthStencilFormat = TEX_FORMAT_DEPTH_STENCIL;
		desc.Width = Window::GetCurrentWindow().GetWidth() / 2;
		desc.Height = Window::GetCurrentWindow().GetHeight() / 2;
		desc.SamplingParams = params;
		desc.Samples = 2;

		m_pGBufferCBR = new Framebuffer(desc);
	}

	{
		TextureParams params = {};
		params.MinFilter = TEX_PARAM_NEAREST;
		params.MagFilter = TEX_PARAM_NEAREST;
		params.Wrap = TEX_PARAM_EDGECLAMP;

		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		//desc.ColorAttchmentFormats[1] = TEX_FORMAT_R32F;
		desc.NumColorAttachments = 1;
		desc.Width = m_pGBufferCBR->GetWidth() * 2;
		desc.Height = m_pGBufferCBR->GetHeight();
		desc.SamplingParams = params;
		desc.Samples = 1;

		m_pResolveTarget = new Framebuffer(desc);
	}

	{
		TextureParams params = {};
		params.Wrap = TEX_PARAM_EDGECLAMP;
		params.MinFilter = TEX_PARAM_LINEAR;
		params.MagFilter = TEX_PARAM_LINEAR;

		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 1;
		desc.SamplingParams = params;
		desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
		desc.Width = Window::GetCurrentWindow().GetWidth();
		desc.Height = Window::GetCurrentWindow().GetHeight();

		m_pBlur = new Framebuffer(desc);
	}

	//CREATE MESHES NEEDED
	{
		m_pDecalMesh = ResourceHandler::GetMesh(MESH::CUBE);
		m_pTriangle = new FullscreenTri();
	}

	//CREATE SHADERPROGRAMS
	Shader fullscreenTri;
	if (fullscreenTri.CompileFromFile("Resources/Shaders/fullscreenTriVert.glsl", VERTEX_SHADER))
	{
		std::cout << "Created fullscreen Vertex shader" << std::endl;
	}

	{
		Shader frag;
		if (frag.CompileFromFile("Resources/Shaders/cbrResolveFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created CBR Resolve Fragment shader" << std::endl;
		}

		m_pCbrResolveProgram = new ShaderProgram(fullscreenTri, frag);
	}

	{
		Shader frag;
		if (frag.CompileFromFile("Resources/Shaders/cbrReconstructionFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created CBR Reconstruction Fragment shader" << std::endl;
		}

		m_pCbrReconstructionProgram = new ShaderProgram(fullscreenTri, frag);
	}

	{
		Shader frag;
		if (frag.CompileFromFile("Resources/Shaders/cbrFilterFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created CBR Blur Fragment shader" << std::endl;
		}

		m_pCbrBlurProgram = new ShaderProgram(fullscreenTri, frag);
	}

	{
		Shader vert;
		if (vert.CompileFromFile("Resources/Shaders/defferedDepthPreVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created DepthPrePass Vertex shader" << std::endl;
		}

		m_pDepthPrePassProgram = new ShaderProgram(vert);
	}

	{
		Shader vert;
		if (vert.CompileFromFile("Resources/Shaders/deferredDecals.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Decal Vertex shader" << std::endl;
		}

		Shader frag;
		if (frag.CompileFromFile("Resources/Shaders/deferredDecals.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Decal Fragment shader" << std::endl;
		}

		m_pDecalsPassProgram = new ShaderProgram(vert, frag);
	}

	{
		Shader vert;
		if (vert.CompileFromFile("Resources/Shaders/forwardVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Forward-Pass Vertex shader" << std::endl;
		}

		Shader frag;
		if (frag.CompileFromFile("Resources/Shaders/forwardFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Forward-Pass Fragment shader" << std::endl;
		}

		m_pForwardPass = new ShaderProgram(vert, frag);
	}

	{
		Shader vert;
		if (vert.CompileFromFile("Resources/Shaders/VShaderSkyBox.glsl", VERTEX_SHADER))
		{
			std::cout << "Created SkyBox pass Vertex shader" << std::endl;
		}

		Shader frag;
		if (frag.CompileFromFile("Resources/Shaders/FShaderSkyBox.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created SkyBox pass Fragment shader" << std::endl;
		}

		m_pSkyBoxPassProgram = new ShaderProgram(vert, frag);
	}

	//CREATE UNIFORMBUFFERS
	{
		CameraBuffer buff = {};
		buff.InverseView = glm::mat4(1.0f);
		buff.InverseProjection = glm::mat4(1.0f);
		buff.CameraPosition = glm::vec3();

		m_pCameraBuffer = new UniformBuffer(&buff, 1, sizeof(CameraBuffer));
	}

	{
		MaterialBuffer buff = {};
		buff.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		m_pMaterialBuffer = new UniformBuffer(&buff, 1, sizeof(MaterialBuffer));
	}

	{
		PlaneBuffer buff = {};
		buff.ClipPlane = glm::vec4(0.0f);

		m_pPlaneBuffer = new UniformBuffer(&buff, 1, sizeof(PlaneBuffer));
	}

	{
		LightBuffer buff = {};
		for (uint32 i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
		{
			buff.DirectionalLights[i].Color = glm::vec4(0.0f);
			buff.DirectionalLights[i].Direction = glm::vec3(0.0f);
		}

		for (uint32 i = 0; i < NUM_POINT_LIGHTS; i++)
		{
			buff.PointLights[i].Color = glm::vec4(0.0f);
			buff.PointLights[i].Position = glm::vec3(0.0f);
		}

		for (uint32 i = 0; i < NUM_SPOT_LIGHTS; i++)
		{
			buff.SpotLights[i].Color = glm::vec4(0.0f);
			buff.SpotLights[i].Position = glm::vec3(0.0f);
			buff.SpotLights[i].Direction = glm::vec3(0.0f);
			buff.SpotLights[i].CutOffAngle = 0.0f;
			buff.SpotLights[i].OuterCutOffAngle = 0.0f;
		}

		m_pLightBuffer = new UniformBuffer(&buff, 1, sizeof(LightBuffer));
	}

	{
		SkyBoxPassBuffer buff = {};
		buff.CameraCombined = glm::mat4(1.0f);
		buff.CameraPosition = glm::vec4();

		m_pSkyBoxPassPerFrame = new UniformBuffer(&buff, 1, sizeof(SkyBoxPassBuffer));

		SkyBoxPassPerObject object = {};
		object.model = glm::mat4(1.0f);

		m_pSkyBoxPassPerObject = new UniformBuffer(&object, 1, sizeof(SkyBoxPassPerObject));
	}

	{
		for (uint32 i = 0; i < NUM_CLIP_DISTANCES; i++)
		{
			m_ClipDistances[i] = glm::vec4(0.0f);
		}
	}
}

void DefferedRenderer::DrawScene(const Scene& scene, float dtS) const
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	//Clear last frame's batches
	for (size_t i = 0; i < m_DrawableBatches.size(); i++)
	{
		m_DrawableBatches[i].Instances.clear();
	}

	for (size_t i = 0; i < m_DecalBatches.size(); i++)
	{
		m_DecalBatches[i].Instances.clear();
	}

	//Create batches for drawables
	//Dahlsson är detta verkligen det mest optimierade du kan göra?
	//-Nej men får duga tills vidare
	{
		const std::vector<GameObject*>& drawables = scene.GetDrawables();
		for (size_t i = 0; i < drawables.size(); i++)
		{
			bool batchFound = false;
			const Material* pMaterial = drawables[i]->GetMaterial();
			const IndexedMesh* pMesh = drawables[i]->GetMesh();
			
			InstanceData instance = {};
			instance.Model = drawables[i]->GetTransform();
			instance.InverseModel = drawables[i]->GetInverseTransform();
		
			for (size_t j = 0; j < m_DrawableBatches.size(); j++)
			{
				if (pMaterial == m_DrawableBatches[j].pMaterial && pMesh == m_DrawableBatches[j].pMesh)
				{
					m_DrawableBatches[j].Instances.push_back(instance);
					batchFound = true;
					break;
				}
			}

			if (!batchFound)
			{
				DrawableBatch batch = {};
				batch.pMaterial = pMaterial;
				batch.pMesh = pMesh;
				batch.Instances.push_back(instance);
				
				m_DrawableBatches.push_back(batch);
			}
		}
	}

	//Create batches for decals
	{
		const std::vector<GameObject*>& decals = scene.GetDecals();
		for (size_t i = 0; i < decals.size(); i++)
		{
			bool batchFound = false;
			const Decal* pDecal = decals[i]->GetDecal();
			const glm::mat4& transform = decals[i]->GetTransform();

			InstanceData instance = {};
			instance.Model = decals[i]->GetTransform();
			instance.InverseModel = decals[i]->GetInverseTransform();
			instance.Direction = instance.Model * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

			for (size_t j = 0; j < m_DecalBatches.size(); j++)
			{
				if (pDecal == m_DecalBatches[j].pDecal)
				{
					m_DecalBatches[j].Instances.push_back(instance);
					batchFound = true;
					break;
				}
			}

			if (!batchFound)
			{
				DecalBatch batch = {};
				batch.pDecal = pDecal;
				batch.Instances.push_back(instance);

				m_DecalBatches.push_back(batch);
			}
		}
	}

	//Set depth and clear color
	context.SetClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	context.SetClearDepth(1.0f);
	context.SetClearStencil(0x00);

	//Update Lightbuffer
	UpdateLightBuffer(scene);

	//Setup for start rendering
	context.Enable(DEPTH_TEST);
	context.Enable(CULL_FACE);
	context.Disable(BLEND);

	//Render reflections
	ReflectionPass(scene);
	
	//Update camera buffer from scene
	UpdateCameraBuffer(scene.GetCamera());

	//Render geometry to MSAA targets for checkerboard rendering
	context.Enable(MULTISAMPLE);
	context.SetViewport(m_pGBufferCBR->GetWidth(), m_pGBufferCBR->GetHeight(), 0, 0);
	context.SetFramebuffer(m_pGBufferCBR);
	context.SetStencilMask(~0); //Active write to stencil or clear stencil does not work
	context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH | CLEAR_FLAG_STENCIL);

	//First the deffered rendering passes
	//DepthPrePass(scene);
	SkyBoxPass(scene.GetCamera(), scene);
	GeometryPass(scene.GetCamera(), scene);
	DecalPass(scene.GetCamera(), scene);
	
	context.Disable(MULTISAMPLE);

	//Resolve the gbuffer (aka we render the rendertargets to a non-MSAA target)
	context.SetFramebuffer(m_pResolveTarget);
	context.SetViewport(m_pResolveTarget->GetWidth(), m_pResolveTarget->GetHeight(), 0, 0);
	context.Disable(DEPTH_TEST);
	
	GBufferResolvePass(scene.GetCamera(), scene, m_pGBufferCBR);

	//Render to the window, now we want to put everything together
	context.SetViewport(Window::GetCurrentWindow().GetWidth(), Window::GetCurrentWindow().GetHeight(), 0, 0);
	context.SetFramebuffer(m_pBlur);
	
	//context.Enable(DEPTH_TEST);
	//context.SetDepthFunc(FUNC_ALWAYS);

	ReconstructionPass();
	
	//context.SetDepthFunc(FUNC_LESS);
}

void DefferedRenderer::UpdateLightBuffer(const Scene& scene) const noexcept
{
	{
		LightBuffer buff = {};

		const std::vector<DirectionalLight*>& directionalLights = scene.GetDirectionalLights();
		for (size_t i = 0; i < directionalLights.size(); i++)
		{
			buff.DirectionalLights[i].Color = directionalLights[i]->GetColor();
			buff.DirectionalLights[i].Direction = directionalLights[i]->GetDirection();
		}

		const std::vector<PointLight*>& pointLights = scene.GetPointLights();
		for (size_t i = 0; i < pointLights.size(); i++)
		{
			buff.PointLights[i].Color = pointLights[i]->GetColor();
			buff.PointLights[i].Position = pointLights[i]->GetPosition();
		}

		const std::vector<SpotLight*>& spotLights = scene.GetSpotLights();
		for (size_t i = 0; i < spotLights.size(); i++)
		{
			buff.SpotLights[i].Color = spotLights[i]->GetColor();
			buff.SpotLights[i].Position = spotLights[i]->GetPosition();
			buff.SpotLights[i].Direction = spotLights[i]->GetDirection();
			buff.SpotLights[i].CutOffAngle = spotLights[i]->GetCutOffAngle();
			buff.SpotLights[i].OuterCutOffAngle = spotLights[i]->GetOuterCutOffAngle();
		}

		m_pLightBuffer->UpdateData(&buff);
	}
}

void DefferedRenderer::UpdateCameraBuffer(const Camera& camera) const noexcept
{
	{
		CameraBuffer buff = {};
		buff.ProjectionView = camera.GetCombinedMatrix();
		buff.View = camera.GetViewMatrix();
		buff.Projection = camera.GetProjectionMatrix();
		buff.InverseView = camera.GetInverseViewMatrix();
		buff.InverseProjection = camera.GetInverseProjectionMatrix();
		buff.CameraLookAt = camera.GetLookAt();
		buff.CameraPosition = camera.GetPosition();

		m_pCameraBuffer->UpdateData(&buff);
	}
}

void DefferedRenderer::DepthPrePass(const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(m_pDepthPrePassProgram);

	context.SetColorMask(0, 0, 0, 0);
	context.SetDepthMask(true);
	context.SetDepthFunc(FUNC_LESS);

	MaterialBuffer perBatch = {};
	for (size_t i = 0; i < m_DrawableBatches.size(); i++)
	{
		const IndexedMesh& mesh = *m_DrawableBatches[i].pMesh;
		
		context.Enable(CULL_FACE);
		
		mesh.SetInstances(m_DrawableBatches[i].Instances.data(), m_DrawableBatches[i].Instances.size());
		context.DrawIndexedMeshInstanced(mesh);

		context.Disable(CULL_FACE);
	}
	
	context.SetDepthMask(false);
	context.SetDepthFunc(FUNC_LESS_EQUAL);
	context.SetColorMask(1, 1, 1, 1);

	context.SetDepthMask(false);
}

void DefferedRenderer::DecalPass(const Camera& camera, const Scene& scene) const noexcept
{
	if (scene.GetDecals().size() < 1)
	{
#if defined(_DEBUG)
		std::cout << "No decals, skipping decalspass" << std::endl;
#endif
		return;
	}

	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(m_pDecalsPassProgram);
	
	context.SetDepthMask(false);
	context.Disable(DEPTH_TEST);
	context.Disable(CULL_FACE);
	context.Enable(BLEND);

	context.SetUniformBuffer(m_pCameraBuffer, CAMERA_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(m_pMaterialBuffer, MATERIAL_BUFFER_BINDING_SLOT);

	context.SetTexture(m_pGBufferCBR->GetDepthAttachment(), 3);

	MaterialBuffer perBatch = {};
	for (size_t i = 0; i < m_DecalBatches.size(); i++)
	{
		const Decal& decal = *m_DecalBatches[i].pDecal;
		if (decal.HasTexture())
		{
			perBatch.HasDiffuseMap = 1.0f;
			context.SetTexture(decal.GetTexture(), DIFFUSE_MAP_BINDING_SLOT);
		}
		else
		{
			perBatch.HasDiffuseMap = 0.0f;
		}

		if (decal.HasNormalMap())
		{
			perBatch.HasNormalMap = 1.0f;
			context.SetTexture(decal.GetNormalMap(), NORMAL_MAP_BINDING_SLOT);
		}
		else
		{
			perBatch.HasNormalMap = 0.0f;
		}

		m_pMaterialBuffer->UpdateData(&perBatch);

		m_pDecalMesh->SetInstances(m_DecalBatches[i].Instances.data(), m_DecalBatches[i].Instances.size());
		context.DrawIndexedMeshInstanced(*m_pDecalMesh);
	}

	//Unbind = no bugs
	context.SetUniformBuffer(nullptr, CAMERA_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(nullptr, MATERIAL_BUFFER_BINDING_SLOT);

	context.SetTexture(nullptr, DIFFUSE_MAP_BINDING_SLOT);
	context.SetTexture(nullptr, NORMAL_MAP_BINDING_SLOT);
	context.SetTexture(nullptr, 3);

	context.SetDepthMask(true);
	context.Disable(BLEND);
	context.Enable(CULL_FACE);
	context.Enable(DEPTH_TEST);
}

void DefferedRenderer::GBufferResolvePass(const Camera& camera, const Scene& scene, const Framebuffer* const pGBuffer)  const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(m_pCbrResolveProgram);

	context.SetUniformBuffer(m_pCameraBuffer, CAMERA_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(m_pLightBuffer, LIGHT_BUFFER_BINDING_SLOT);

	context.SetTexture(pGBuffer->GetColorAttachment(0), 0); //color buffer
	context.SetTexture(pGBuffer->GetColorAttachment(1), 1); //normal buffer
	context.SetTexture(pGBuffer->GetDepthAttachment(), 2);  //depth buffer

	context.DrawFullscreenTriangle(*m_pTriangle);

	//Unbind resources = no bugs
	context.SetUniformBuffer(nullptr, CAMERA_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(nullptr, LIGHT_BUFFER_BINDING_SLOT);

	context.SetTexture(nullptr, 0);
	context.SetTexture(nullptr, 1);
	context.SetTexture(nullptr, 2);
}

void DefferedRenderer::ReconstructionPass() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetProgram(m_pCbrReconstructionProgram);

	context.SetTexture(m_pResolveTarget->GetColorAttachment(0), 0);	//Color

	context.DrawFullscreenTriangle(*m_pTriangle);

	context.SetProgram(m_pCbrBlurProgram);

	context.SetFramebuffer(nullptr);
	context.SetTexture(m_pBlur->GetColorAttachment(0), 0);
	
	context.DrawFullscreenTriangle(*m_pTriangle);

	//Unbind = no bugs
	context.SetTexture(nullptr, 0);
}

void DefferedRenderer::GeometryPass(const Camera& camera, const Scene& scene) const noexcept
{
	if (m_DrawableBatches.size() < 1)
	{
#if defined(_DEBUG)
		std::cout << "No drawables, skipping geomtrypass" << std::endl;
#endif
		return;
	}

	GLContext& context = GLContext::GetCurrentContext();

	MaterialBuffer perBatch = {};
	for (size_t i = 0; i < m_DrawableBatches.size(); i++)
	{
		const IndexedMesh& mesh = *m_DrawableBatches[i].pMesh;
		const Material& material = *m_DrawableBatches[i].pMaterial;

		perBatch.Color = material.GetColor();
		perBatch.ClipPlane = material.GetLevelClipPlane();
		perBatch.Specular = material.GetSpecular();
		perBatch.HasDiffuseMap = material.HasDiffuseMap() ? 1.0f : 0.0f;
		perBatch.HasNormalMap = material.HasNormalMap() ? 1.0f : 0.0f;
		perBatch.HasSpecularMap = material.HasSpecularMap() ? 1.0f : 0.0f;
		m_pMaterialBuffer->UpdateData(&perBatch);

		material.SetCameraBuffer(m_pCameraBuffer);
		material.SetLightBuffer(m_pLightBuffer);
		material.SetMaterialBuffer(m_pMaterialBuffer);
		material.Bind(m_pGBufferCBR);

		mesh.SetInstances(m_DrawableBatches[i].Instances.data(), m_DrawableBatches[i].Instances.size());
		context.DrawIndexedMeshInstanced(mesh);

		context.Enable(CULL_FACE);
		
		material.Unbind();
	}
}

void DefferedRenderer::ForwardPass(const Camera& camera, const Scene& scene) const noexcept
{
	if (m_DrawableBatches.size() < 1)
	{
#if defined(_DEBUG)
		std::cout << "No drawables, skipping forwardpass" << std::endl;
#endif
		return;
	}

	GLContext& context = GLContext::GetCurrentContext();

	context.Enable(CLIP_DISTANCE0);

	context.SetProgram(m_pForwardPass);

	context.SetUniformBuffer(m_pCameraBuffer, 0);
	context.SetUniformBuffer(m_pLightBuffer, 1);
	context.SetUniformBuffer(m_pMaterialBuffer, 2);
	context.SetUniformBuffer(m_pPlaneBuffer, 3);

	MaterialBuffer matBuffer = {};
	for (size_t i = 0; i < m_DrawableBatches.size(); i++)
	{
		const IndexedMesh& mesh = *m_DrawableBatches[i].pMesh;
		const Material& material = *m_DrawableBatches[i].pMaterial;
		
		matBuffer.Color = material.GetColor();
		matBuffer.Specular = material.GetSpecular();
		if (material.HasDiffuseMap())
		{
			context.SetTexture(material.GetDiffuseMap(), 0);
			matBuffer.HasDiffuseMap = 1.0f;
		}
		else
		{
			matBuffer.HasDiffuseMap = 0.0f;
		}
		
		if (material.HasNormalMap())
		{
			context.SetTexture(material.GetNormalMap(), 1);
			matBuffer.HasNormalMap = 1.0f;
		}
		else
		{
			matBuffer.HasNormalMap = 0.0f;
		}
		
		if (material.HasSpecularMap())
		{
			context.SetTexture(material.GetSpecularMap(), 2);
			matBuffer.HasSpecularMap = 1.0f;
		}
		else
		{
			matBuffer.HasSpecularMap = 0.0f;
		}
		
		m_pMaterialBuffer->UpdateData(&matBuffer);

		mesh.SetInstances(m_DrawableBatches[i].Instances.data(), m_DrawableBatches[i].Instances.size());
		context.DrawIndexedMeshInstanced(mesh);
	}

	context.Disable(CLIP_DISTANCE0);
	
	//Unbind = no bugs
	context.SetUniformBuffer(nullptr, 0);
	context.SetUniformBuffer(nullptr, 1);
	context.SetUniformBuffer(nullptr, 2);
	context.SetUniformBuffer(nullptr, 3);

	context.SetTexture(nullptr, 0);
	context.SetTexture(nullptr, 1);
	context.SetTexture(nullptr, 2);
}

void DefferedRenderer::ReflectionPass(const Scene& scene) const noexcept
{
	if (scene.GetReflectables().size() < 1 || scene.GetPlanarReflectors().size() < 1)
	{
#if defined(_DEBUG)
		std::cout << "No reflectables or reflectors, skipping reflectionpass" << std::endl;
#endif
		return;
	}

	GLContext& context = Application::GetInstance().GetGraphicsContext();

	Camera reflectionCam = scene.GetCamera();
	float reflDistance = reflectionCam.GetPosition().y * 2.0f;
	reflectionCam.SetPos(reflectionCam.GetPosition() - glm::vec3(0.0f, reflDistance, 0.0f));
	reflectionCam.InvertPitch();
	reflectionCam.UpdateFromPitchYawNoInverse();

	PlaneBuffer planeBuffer = {};

	UpdateCameraBuffer(reflectionCam);

	const std::vector<PlanarReflector*>& reflectors = scene.GetPlanarReflectors();
	for (size_t i = 0; i < reflectors.size(); i++)
	{
		const Framebuffer* pFramebuffer = reflectors[i]->GetFramebuffer();
	
		planeBuffer.ClipPlane = reflectors[i]->GetLevelClipPlane();
		m_pPlaneBuffer->UpdateData(&planeBuffer);

		context.SetViewport(pFramebuffer->GetWidth(), pFramebuffer->GetHeight(), 0, 0);
		context.SetFramebuffer(pFramebuffer);
		context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

		ForwardPass(reflectionCam, scene);
	}
}

void DefferedRenderer::SkyBoxPass(const Camera& camera, const Scene& screen) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();
	context.SetProgram(m_pSkyBoxPassProgram);

	context.Disable(CULL_FACE);
	context.Disable(BLEND);
	context.Disable(DEPTH_TEST);

	SkyBoxPassBuffer perFrame = {};
	perFrame.CameraCombined = camera.GetProjectionMatrix() * glm::mat4(glm::mat3(camera.GetViewMatrix()));
	perFrame.CameraPosition = glm::vec4(camera.GetPosition(), 1.0f);
	m_pSkyBoxPassPerFrame->UpdateData(&perFrame);

	SkyBoxPassPerObject perObject;
	perObject.model = glm::mat4(1.0f);
	m_pSkyBoxPassPerObject->UpdateData(&perObject);

	context.SetTexture(&screen.GetSkyBox().GetTexture(),0);

	context.SetUniformBuffer(m_pSkyBoxPassPerObject, 0);
	context.SetUniformBuffer(m_pSkyBoxPassPerFrame, 1);

	context.DrawIndexedMesh(screen.GetSkyBox().GetMesh());

	context.Enable(DEPTH_TEST);
	context.Enable(CULL_FACE);
}
