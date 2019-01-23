#include "..\Include\Game.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Renderers/DefferedRenderer.h>

Game::Game() noexcept
	: m_pRenderer(nullptr)
{
	Shader vShaderDefault;
	Shader fShaderDefault;

	vShaderDefault.CompileFromFile("Resources/Shaders/VShader.glsl", VERTEX_SHADER);
	fShaderDefault.CompileFromFile("Resources/Shaders/FShader.glsl", FRAGMENT_SHADER);

	m_pShaderProgramDefault = new ShaderProgram(vShaderDefault, fShaderDefault);

	Shader vShaderWater;
	Shader fShaderWater;

	vShaderWater.CompileFromFile("Resources/Shaders/VShaderWater.glsl", VERTEX_SHADER);
	fShaderWater.CompileFromFile("Resources/Shaders/FShaderWater.glsl", FRAGMENT_SHADER);

	m_pShaderProgramWater = new ShaderProgram(vShaderWater, fShaderWater);

	m_pScene = new Scene();
	m_pTestMesh = IndexedMesh::CreateIndexedMeshFromFile("Resources/Meshes/ship.obj");

	m_pRenderer = new DefferedRenderer();

	GameObject* pGameObject = nullptr;
	for (unsigned int i = 0; i < 25; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(m_pTestMesh);
		pGameObject->SetPosition(5.0f * (glm::vec3(i / 5, -0.02f, i % 5) + glm::vec3(-2.5f, 0.0f, -2.5f)));
		pGameObject->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		pGameObject->UpdateTransform();
		m_pScene->AddGameObject(pGameObject);
		m_GameObjectUniforms.push_back(new UniformBuffer(glm::value_ptr(pGameObject->GetTransform()), 1, sizeof(glm::mat4)));
	}

	Camera* pCamera = new Camera(glm::vec3(-2.0F, 1.0F, 0.0F), -0.5f, 0.0f);
	pCamera->SetProjectionMatrix(glm::perspective(
		glm::radians<float>(90.0F),
		(float)GetWindow().GetWidth() /
		(float)GetWindow().GetHeight(),
		0.1F, 100.0F));
	pCamera->Update();
	m_pScene->SetCamera(pCamera);

	m_pScene->GetCamera().CopyShaderDataToArray(m_PerFrameArray, 0);
	m_pPerFrameUniform = new UniformBuffer(m_PerFrameArray, 1, sizeof(m_PerFrameArray));

	//Water Stuff
	m_pWaterMesh = IndexedMesh::CreateQuad();

	m_pWaterGameObject = new GameObject();
	m_pWaterGameObject->SetMesh(m_pWaterMesh);
	m_pWaterGameObject->SetScale(glm::vec3(15.0f));
	m_pWaterGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::half_pi<float>()));
	m_pWaterGameObject->UpdateTransform();

	m_pWaterUniform = new UniformBuffer(glm::value_ptr(m_pWaterGameObject->GetTransform()), 1, sizeof(glm::mat4));

	m_WaterTextureParams.Wrap = TEX_PARAM_REPEAT;
	m_WaterTextureParams.MinFilter = TEX_LINEAR;
	m_WaterTextureParams.MagFilter = TEX_LINEAR;

	FramebufferDesc fboDescReflRefr;
	fboDescReflRefr.ColorAttchmentFormats[0] = TEX_FORMAT::TEX_FORMAT_RGBA;
	fboDescReflRefr.NumColorAttachments = 1;
	fboDescReflRefr.SamplingParams = m_WaterTextureParams;
	fboDescReflRefr.DepthStencilFormat = TEX_FORMAT::TEX_FORMAT_DEPTH;
	fboDescReflRefr.Width = GetWindow().GetWidth();
	fboDescReflRefr.Height = GetWindow().GetHeight();

	m_pReflectionFBO = new Framebuffer(fboDescReflRefr);
	m_pRefractionFBO = new Framebuffer(fboDescReflRefr);

	m_pDUDVTexture = new Texture2D("Resources/Textures/waterDUDV.png", TEX_FORMAT::TEX_FORMAT_RGBA, false, m_WaterTextureParams);

	GetContext().Enable(Cap::DEPTH_TEST);
	GetContext().Enable(Cap::CULL_FACE);
}

Game::~Game()
{
	delete m_pRenderer;
	delete m_pShaderProgramDefault;
	delete m_pTestMesh;
	delete m_pScene;

	delete m_pReflectionFBO;
	delete m_pRefractionFBO;
}

void Game::OnMouseMove(const glm::vec2& position)
{
}

void Game::OnUpdate(float dtS)
{
	static float tempRotation = 0.0f;
	tempRotation += 1.0f * dtS;

	for (uint32 i = 0; i < 25; i++)
	{
		//m_pScene->GetGameObjects()[i]->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, tempRotation));
		m_pScene->GetGameObjects()[i]->UpdateTransform();
		m_GameObjectUniforms[i]->UpdateData(glm::value_ptr(m_pScene->GetGameObjects()[i]->GetTransform()));
	}

	static float cameraSpeed = 5.0f;
	static float angularSpeed = 1.5f;

	if (Input::IsKeyDown(KEY_W))
	{
		m_pScene->GetCamera().Move(CameraDir::Forward, cameraSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_S))
	{
		m_pScene->GetCamera().Move(CameraDir::Backwards, cameraSpeed * dtS);
	}

	if (Input::IsKeyDown(KEY_A))
	{
		m_pScene->GetCamera().Move(CameraDir::Left, cameraSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_D))
	{
		m_pScene->GetCamera().Move(CameraDir::Right, cameraSpeed * dtS);
	}

	if (Input::IsKeyDown(KEY_E))
	{
		m_pScene->GetCamera().Move(CameraDir::Up, cameraSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_Q))
	{
		m_pScene->GetCamera().Move(CameraDir::Down, cameraSpeed * dtS);
	}

	if (Input::IsKeyDown(KEY_UP))
	{
		m_pScene->GetCamera().OffsetPitch(angularSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_DOWN))
	{
		m_pScene->GetCamera().OffsetPitch(-angularSpeed * dtS);
	}

	if (Input::IsKeyDown(KEY_LEFT))
	{
		m_pScene->GetCamera().OffsetYaw(-angularSpeed * dtS);
	}
	else if (Input::IsKeyDown(KEY_RIGHT))
	{
		m_pScene->GetCamera().OffsetYaw(angularSpeed * dtS);
	}
	
	m_pScene->GetCamera().Update();
	m_pScene->GetCamera().CopyShaderDataToArray(m_PerFrameArray, 0);
	m_pPerFrameUniform->UpdateData(&m_PerFrameArray);

	m_DistortionMoveFactor += 0.02f * dtS;
	m_DistortionMoveFactor = fmodf(m_DistortionMoveFactor, 1.0f);

	Application::OnUpdate(dtS);
}

void Game::OnRender()
{
	/*GetContext().SetProgram(m_pShaderProgramDefault);

	assert(m_pScene->GetGameObjects().size() == m_GameObjectUniforms.size());

	GetContext().Enable(Cap::CLIP_DISTANCE0);

	//Draw Scene for reflection
	float reflDistance = m_pScene->GetCamera().GetPosition().y * 2;
	m_pScene->GetCamera().SetPos(m_pScene->GetCamera().GetPosition() - glm::vec3(0.0f, reflDistance, 0.0f));
	m_pScene->GetCamera().InvertPitch();
	m_pScene->GetCamera().Update();

	m_pScene->GetCamera().CopyShaderDataToArray(m_PerFrameArray, 0);
	m_PerFrameArray[20] = 0.0f;
	m_PerFrameArray[21] = 1.0f;
	m_PerFrameArray[22] = 0.0f;
	m_PerFrameArray[23] = 0.0f;
	m_pPerFrameUniform->UpdateData(&m_PerFrameArray);
	GetContext().SetUniformBuffer(m_pPerFrameUniform, 1);

	GetContext().SetFramebuffer(m_pReflectionFBO);
	GetContext().Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);


	for (uint32 i = 0; i < m_GameObjectUniforms.size(); i++)
	{
		GetContext().SetUniformBuffer(m_GameObjectUniforms[i], 0);
		GetContext().DrawIndexedMesh(m_pScene->GetGameObjects()[i]->GetMesh());
	}

	m_pScene->GetCamera().SetPos(m_pScene->GetCamera().GetPosition() + glm::vec3(0.0f, reflDistance, 0.0f));
	m_pScene->GetCamera().InvertPitch();
	m_pScene->GetCamera().Update();

	//Draw Scene for refraction
	m_pScene->GetCamera().CopyShaderDataToArray(m_PerFrameArray, 0);
	m_PerFrameArray[20] = 0.0f;
	m_PerFrameArray[21] = -1.0f;
	m_PerFrameArray[22] = 0.0f;
	m_PerFrameArray[23] = 0.0f;
	m_pPerFrameUniform->UpdateData(&m_PerFrameArray);
	GetContext().SetUniformBuffer(m_pPerFrameUniform, 1);

	GetContext().SetFramebuffer(m_pRefractionFBO);
	GetContext().Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

	for (unsigned int i = 0; i < m_GameObjectUniforms.size(); i++)
	{
		GetContext().SetUniformBuffer(m_GameObjectUniforms[i], 0);
		GetContext().DrawIndexedMesh(m_pScene->GetGameObjects()[i]->GetMesh());
	}

	GetContext().Disable(Cap::CLIP_DISTANCE0);

	//Draw Scene to screen
	GetContext().SetFramebuffer(nullptr);

	for (unsigned int i = 0; i < m_GameObjectUniforms.size(); i++)
	{
		GetContext().SetUniformBuffer(m_GameObjectUniforms[i], 0);
		GetContext().DrawIndexedMesh(m_pScene->GetGameObjects()[i]->GetMesh());
	}

	//Draw Water to screen
	GetContext().SetProgram(m_pShaderProgramWater);
	m_PerFrameArray[19] = m_DistortionMoveFactor; //Per Frame Fistortion Move Factor
	m_pPerFrameUniform->UpdateData(&m_PerFrameArray);
	GetContext().SetUniformBuffer(m_pPerFrameUniform, 1);
	GetContext().SetUniformBuffer(m_pWaterUniform, 0);

	GetContext().SetTexture(m_pReflectionFBO->GetColorAttachment(0), 0);
	GetContext().SetTexture(m_pRefractionFBO->GetColorAttachment(0), 1);
	GetContext().SetTexture(m_pDUDVTexture, 2);

	GetContext().DrawIndexedMesh(m_pWaterGameObject->GetMesh());*/

	m_pRenderer->DrawScene(*m_pScene);
	Application::OnRender();
}
