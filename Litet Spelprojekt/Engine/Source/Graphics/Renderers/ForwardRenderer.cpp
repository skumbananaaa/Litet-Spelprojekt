#include <EnginePch.h>
#include <System/Application.h>
#include <Graphics/Renderers/ForwardRenderer.h>
#include <Graphics/Renderers/GLContext.h>

ForwardRenderer::ForwardRenderer()
	: m_pLightBuffer(nullptr),
	m_pCameraBuffer(nullptr),
	m_pMaterialBuffer(nullptr),
	m_pPlaneBuffer(nullptr),
	m_pSkyBoxPassPerFrame(nullptr),
	m_pSkyBoxPassPerObject(nullptr),
	m_pParticle(nullptr),
	m_pParticleProgram(nullptr),
	m_pDepthPrePassProgram(nullptr),
	m_pSkyBoxPassProgram(nullptr),
	m_pCurrentQuery(nullptr),
	m_FrameCounter(0),
	m_FrameTimes(),
	m_pQueries(),
	m_DrawableBatches()
{
	m_pQueries[0] = nullptr;
	m_pQueries[1] = nullptr;

	Create();
}

ForwardRenderer::~ForwardRenderer()
{
	DeleteSafe(m_pLightBuffer);
	DeleteSafe(m_pCameraBuffer);
	DeleteSafe(m_pMaterialBuffer);
	DeleteSafe(m_pPlaneBuffer);
	DeleteSafe(m_pSkyBoxPassPerFrame);
	DeleteSafe(m_pSkyBoxPassPerObject);
	DeleteSafe(m_pParticle);
	DeleteSafe(m_pQueries[0]);
	DeleteSafe(m_pQueries[1]);
}

void ForwardRenderer::DrawScene(const Scene& scene, float dtS) const
{
	static float timer = 0.0f;
	static float frametime = 0.0f;
	frametime += dtS;
	timer += dtS;

	GLContext& context = Application::GetInstance().GetGraphicsContext();

	//Set current query
	m_pCurrentQuery = m_pQueries[m_FrameCounter % 2];

	//Clear last frame's batches
	for (size_t i = 0; i < m_DrawableBatches.size(); i++)
	{
		m_DrawableBatches[i].Instances.clear();
	}

	//Create batches
	CreateBatches(scene);
	//Update lights
	UpdateLightBuffer(scene);

	//Reflections
	glQueryCounter(m_pCurrentQuery->pQueries[0], GL_TIMESTAMP);
	context.SetDepthMask(true);
	ReflectionPass(scene);
	glQueryCounter(m_pCurrentQuery->pQueries[1], GL_TIMESTAMP);

	//Update camrerabuffer to main camera
	const Camera& mainCamera = scene.GetCamera();
	UpdateCameraBuffer(mainCamera);

	//Set viewport and framebuffer
	context.SetFramebuffer(nullptr);
	context.SetViewport(Window::GetCurrentWindow().GetWidth(), Window::GetCurrentWindow().GetHeight(), 0, 0);
	context.SetStencilMask(~0); //Active write to stencil or clear stencil does not work
	context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH | CLEAR_FLAG_STENCIL);

	//Render scene
	glQueryCounter(m_pCurrentQuery->pQueries[2], GL_TIMESTAMP);
	context.SetDepthFunc(FUNC_LESS);

	DepthPrePass(mainCamera, scene);
	
	context.SetDepthMask(false);
	context.SetDepthFunc(FUNC_LESS_EQUAL);
	glQueryCounter(m_pCurrentQuery->pQueries[3], GL_TIMESTAMP);
	MainPass(mainCamera, scene);
	glQueryCounter(m_pCurrentQuery->pQueries[4], GL_TIMESTAMP);
	SkyBoxPass(mainCamera, scene);
	glQueryCounter(m_pCurrentQuery->pQueries[5], GL_TIMESTAMP);
	ParticlePass(mainCamera, scene);
	glQueryCounter(m_pCurrentQuery->pQueries[6], GL_TIMESTAMP);

	//Get query results
	uint64 startTime = 0;
	uint64 stopTime = 0;
	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[0], GL_QUERY_RESULT, &startTime);
	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[1], GL_QUERY_RESULT, &stopTime);
	m_FrameTimes.ReflectionPass += static_cast<float>(stopTime - startTime) / 1000000.0f;

	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[2], GL_QUERY_RESULT, &startTime);
	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[3], GL_QUERY_RESULT, &stopTime);
	m_FrameTimes.DepthPrePass += static_cast<float>(stopTime - startTime) / 1000000.0f;
	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[4], GL_QUERY_RESULT, &startTime);
	m_FrameTimes.LightPass += static_cast<float>(startTime - stopTime) / 1000000.0f;
	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[5], GL_QUERY_RESULT, &stopTime);
	m_FrameTimes.SkyboxPass += static_cast<float>(stopTime - startTime) / 1000000.0f;
	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[6], GL_QUERY_RESULT, &startTime);
	m_FrameTimes.ParticlePass += static_cast<float>(startTime - stopTime) / 1000000.0f;

	if (timer >= 1.0f)
	{
		float fps = static_cast<float>(Application::GetInstance().GetFPS());

		std::cout << "Frametimes: (Total time: " << (frametime / fps) * 1000.0f << "ms) " << std::endl;
		std::cout << " Reflectionpass: " << m_FrameTimes.ReflectionPass / fps << "ms" << std::endl;
		std::cout << " DepthPrePass: " << m_FrameTimes.DepthPrePass / fps << "ms" << std::endl;
		std::cout << " Lightpass: " << m_FrameTimes.LightPass / fps << "ms" << std::endl;
		std::cout << " Skyboxpass: " << m_FrameTimes.SkyboxPass / fps << "ms" << std::endl;
		std::cout << " Particlepass: " << m_FrameTimes.ParticlePass / fps << "ms" << std::endl;
		std::cout << "-----------" << std::endl;

		frametime = 0.0f;
		m_FrameTimes.ReflectionPass = 0.0f;
		m_FrameTimes.SkyboxPass = 0.0f;
		m_FrameTimes.ParticlePass = 0.0f;
		m_FrameTimes.LightPass = 0.0f;
		m_FrameTimes.DepthPrePass = 0.0f;

		timer = 0.0f;
	}

	m_FrameCounter++;
}

void ForwardRenderer::Create() noexcept
{
	std::cout << "Creating forward renderer" << std::endl;

	//CREATE QUERIES
	m_pQueries[0] = new TimerQuery(7);
	m_pQueries[1] = new TimerQuery(7);

	//CREATE MESHES NEEDED
	{
		m_pParticle = new Particle();
	}

	m_pDepthPrePassProgram = ResourceHandler::GetShader(SHADER::DEPTH_PRE_PASS);
	m_pSkyBoxPassProgram = ResourceHandler::GetShader(SHADER::SKYBOX_PASS);
	m_pParticleProgram = ResourceHandler::GetShader(SHADER::PARTICLES);

	//We can destroy object when uniformbuffer is created
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
}

void ForwardRenderer::CreateBatches(const Scene& scene) const noexcept
{
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
}

void ForwardRenderer::UpdateLightBuffer(const Scene& scene) const noexcept
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

void ForwardRenderer::SetClipDistance(const glm::vec4& plane, uint32 index)
{
}

void ForwardRenderer::UpdateCameraBuffer(const Camera& camera) const noexcept
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

void ForwardRenderer::ReflectionPass(const Scene& scene) const noexcept
{
	if (scene.GetReflectables().size() < 1 || scene.GetPlanarReflectors().size() < 1)
	{
#if defined(_DEBUG)
		//std::cout << "No reflectables or reflectors, skipping reflectionpass" << std::endl;
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

	context.Enable(CLIP_DISTANCE1);
	context.SetUniformBuffer(m_pPlaneBuffer, PLANE_BUFFER_BINDING_SLOT);

	const std::vector<PlanarReflector*>& reflectors = scene.GetPlanarReflectors();
	for (size_t i = 0; i < reflectors.size(); i++)
	{
		const Framebuffer* pFramebuffer = reflectors[i]->GetFramebuffer();

		planeBuffer.ClipPlane = reflectors[i]->GetLevelClipPlane();
		m_pPlaneBuffer->UpdateData(&planeBuffer);

		context.SetViewport(pFramebuffer->GetWidth(), pFramebuffer->GetHeight(), 0, 0);
		context.SetFramebuffer(pFramebuffer);
		context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

		MainPass(reflectionCam, scene);
	}

	context.SetUniformBuffer(nullptr, PLANE_BUFFER_BINDING_SLOT);
	context.Disable(CLIP_DISTANCE1);
}

void ForwardRenderer::DepthPrePass(const Camera& camera, const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(m_pDepthPrePassProgram);

	context.SetColorMask(0, 0, 0, 0);
	context.Enable(CLIP_DISTANCE0);

	context.SetUniformBuffer(m_pCameraBuffer, CAMERA_BUFFER_BINDING_SLOT);
	context.SetUniformBuffer(m_pPlaneBuffer, PLANE_BUFFER_BINDING_SLOT);

	MaterialBuffer perBatch = {};
	for (size_t i = 0; i < m_DrawableBatches.size(); i++)
	{
		const IndexedMesh& mesh = *m_DrawableBatches[i].pMesh;
		const Material& material = *m_DrawableBatches[i].pMaterial;

		PlaneBuffer buff = {};
		buff.ClipPlane = material.GetLevelClipPlane();
		m_pPlaneBuffer->UpdateData(&buff);

		if (material.GetCullMode() != CULL_MODE_NONE)
		{
			context.Enable(CULL_FACE);
			context.SetCullMode(material.GetCullMode());
		}
		else
		{
			context.Disable(CULL_FACE);
		}

		mesh.SetInstances(m_DrawableBatches[i].Instances.data(), m_DrawableBatches[i].Instances.size());
		context.DrawIndexedMeshInstanced(mesh);
	}

	context.Disable(CLIP_DISTANCE0);
	context.SetColorMask(1, 1, 1, 1);
}

void ForwardRenderer::MainPass(const Camera& camera, const Scene& scene) const noexcept
{
	if (m_DrawableBatches.size() < 1)
	{
#if defined(_DEBUG)
		//std::cout << "No drawables, skipping geomtrypass" << std::endl;
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
		material.Bind(nullptr);

		mesh.SetInstances(m_DrawableBatches[i].Instances.data(), m_DrawableBatches[i].Instances.size());
		context.DrawIndexedMeshInstanced(mesh);

		context.Enable(CULL_FACE);

		material.Unbind();
	}
}

void ForwardRenderer::ParticlePass(const Camera& camera, const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.Disable(CULL_FACE);
	context.Enable(BLEND);

	context.SetProgram(m_pParticleProgram);

	context.SetUniformBuffer(m_pCameraBuffer, CAMERA_BUFFER_BINDING_SLOT);

	const std::vector<ParticleSystem*>& particleSystems = scene.GetParticleSystem();
	for (size_t i = 0; i < particleSystems.size(); i++)
	{
		m_pParticle->SetInstances(particleSystems[i]->GetParticleInstances(), particleSystems[i]->GetNumParticles());

		context.SetTexture(particleSystems[i]->GetTexture(), DIFFUSE_MAP_BINDING_SLOT);
		context.DrawParticle(*m_pParticle);
	}

	context.Enable(CULL_FACE);
	context.Disable(BLEND);
}

void ForwardRenderer::SkyBoxPass(const Camera& camera, const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();
	context.SetProgram(m_pSkyBoxPassProgram);

	context.Disable(CULL_FACE);

	SkyBoxPassBuffer perFrame = {};
	perFrame.CameraCombined = camera.GetProjectionMatrix() * glm::mat4(glm::mat3(camera.GetViewMatrix()));
	perFrame.CameraPosition = glm::vec4(camera.GetPosition(), 1.0f);
	m_pSkyBoxPassPerFrame->UpdateData(&perFrame);

	SkyBoxPassPerObject perObject;
	perObject.model = glm::mat4(1.0f);
	m_pSkyBoxPassPerObject->UpdateData(&perObject);

	context.SetTexture(&scene.GetSkyBox().GetTexture(), 0);

	context.SetUniformBuffer(m_pSkyBoxPassPerObject, 0);
	context.SetUniformBuffer(m_pSkyBoxPassPerFrame, 1);

	context.DrawIndexedMesh(scene.GetSkyBox().GetMesh());

	context.Enable(CULL_FACE);
}
