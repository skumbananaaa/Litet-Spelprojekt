#include "..\Include\Game.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Renderers/DefferedRenderer.h>

Game::Game() noexcept
	: m_pFontRenderer(nullptr),
	m_pRenderer(nullptr),
	m_pScene(nullptr),
	m_pTestMesh(nullptr),
	m_pWaterMesh(nullptr),
	m_pGroundTestMesh(nullptr),
	m_pBoatTexture(nullptr),
	m_pBoatNormalMap(nullptr),
	m_pBloodTexture(nullptr),
	m_pBloodNormal(nullptr),
	m_pBoatMaterial(nullptr),
	m_pGroundMaterial(nullptr),
	m_pDecal(nullptr),
	cartesianCamera(true)
{
	m_pRenderer = new DefferedRenderer();

	m_pScene = new Scene();
	m_pTestMesh = IndexedMesh::CreateIndexedMeshFromFile("Resources/Meshes/ship.obj");
	m_pGroundTestMesh = IndexedMesh::CreateIndexedMeshFromFile("Resources/Meshes/cliff_3_low.obj");

	m_pBoatTexture = new Texture2D("Resources/Textures/ship.jpg", TEX_FORMAT_RGBA);
	m_pBoatNormalMap = new Texture2D("Resources/Textures/shipNormalMap.png", TEX_FORMAT_RGBA);

	m_pBoatMaterial = new Material();
	m_pBoatMaterial->SetColor(glm::vec4(0.655f, 0.639f, 0.627f, 1.0f));
	m_pBoatMaterial->SetTexture(m_pBoatTexture);
	m_pBoatMaterial->SetNormalMap(m_pBoatNormalMap);

	m_pGroundMaterial = new Material();
	m_pGroundMaterial->SetColor(glm::vec4(0.471f, 0.282f, 0.11f, 1.0f));

	m_pBloodTexture = new Texture2D("Resources/Textures/blood.png", TEX_FORMAT_RGBA);
	m_pBloodNormal = new Texture2D("Resources/Textures/bloodNormal.png", TEX_FORMAT_RGBA);

	m_pDecal = new Decal();
	m_pDecal->SetTexture(m_pBloodTexture);
	m_pDecal->SetNormalMap(m_pBloodNormal);

	GameObject* pGameObject = nullptr;

	pGameObject = new GameObject();
	pGameObject->SetDecal(m_pDecal);
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(m_pBoatMaterial);
	pGameObject->SetMesh(m_pTestMesh);
	pGameObject->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	pGameObject->SetScale(glm::vec3(6.0f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(m_pGroundMaterial);
	pGameObject->SetMesh(m_pGroundTestMesh);
	pGameObject->SetPosition(glm::vec3(0.0f, -1.4f, 0.0f));
	pGameObject->SetScale(glm::vec3(0.4f));
	pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::half_pi<float>()));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	Camera* pCamera = new Camera(glm::vec3(-2.0F, 1.0F, 0.0F), glm::vec3(1.0, 0.0, 0.0));
	pCamera->SetProjectionMatrix(glm::perspective(
		glm::radians<float>(90.0F),
		(float)GetWindow().GetWidth() /
		(float)GetWindow().GetHeight(),
		0.1F, 100.0F));
	pCamera->UpdateFromPitchYaw();
	m_pScene->SetCamera(pCamera);

	//Water Stuff
	m_pWaterMesh = IndexedMesh::CreateQuad();

	pGameObject = new GameObject();
	pGameObject->SetMaterial(nullptr);
	pGameObject->SetMesh(m_pWaterMesh);
	pGameObject->SetScale(glm::vec3(15.0f));
	pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::half_pi<float>()));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);


	m_pTextViewFPS = new TextView(0, 720, 200, 50, "FPS");
	m_pTextViewUPS = new TextView(0, 690, 200, 50, "UPS");

	GetGUIManager().AddGUIObject(m_pTextViewFPS);
	GetGUIManager().AddGUIObject(m_pTextViewUPS);
}

Game::~Game()
{
	Delete(m_pBloodTexture);
	Delete(m_pBloodNormal);
	Delete(m_pFontRenderer);
	Delete(m_pRenderer);
	Delete(m_pScene);
	Delete(m_pTestMesh);
	Delete(m_pWaterMesh);
	Delete(m_pGroundTestMesh);
	Delete(m_pBoatTexture);
	Delete(m_pBoatNormalMap);
	Delete(m_pDecal);
	Delete(m_pBoatMaterial);
	Delete(m_pGroundMaterial);
	Delete(m_pTextViewFPS);
	Delete(m_pTextViewUPS);
}

void Game::OnKeyUp(KEY keycode)
{
	Application::OnKeyUp(keycode);
}

void Game::OnKeyDown(KEY keycode)
{
	switch (keycode)
	{
		case KEY_O:
		{
			cartesianCamera = !cartesianCamera;
			break;
		}
	}

	Application::OnKeyDown(keycode);
}

void Game::OnMouseMove(const glm::vec2& position)
{
	Application::OnMouseMove(position);
}

void Game::OnUpdate(float dtS)
{
	if (cartesianCamera)
	{
		//Cartesian
		static float cartesianCameraSpeed = 5.0f;
		static float cartesianCameraAngularSpeed = 1.5f;

		if (Input::IsKeyDown(KEY_W))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Forward, cartesianCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_S))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Backwards, cartesianCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_A))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Left, cartesianCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_D))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Right, cartesianCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_E))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Up, cartesianCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_Q))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Down, cartesianCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_UP))
		{
			m_pScene->GetCamera().OffsetPitch(cartesianCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_DOWN))
		{
			m_pScene->GetCamera().OffsetPitch(-cartesianCameraAngularSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_LEFT))
		{
			m_pScene->GetCamera().OffsetYaw(-cartesianCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_RIGHT))
		{
			m_pScene->GetCamera().OffsetYaw(cartesianCameraAngularSpeed * dtS);
		}

		m_pScene->GetCamera().UpdateFromPitchYaw();
	}
	else
	{
		//Polar
		static float polarCameraSpeed = 5.0f;
		static float polarCameraAngularSpeed = 0.8f;

		if (Input::IsKeyDown(KEY_W))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Forward, polarCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_S))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Backwards, polarCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_A))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Left, polarCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_D))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Right, polarCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_E))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Up, polarCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_Q))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Down, polarCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_UP))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::RotateUp, polarCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_DOWN))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::RotateDown, polarCameraAngularSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_LEFT))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::RotateLeft, polarCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_RIGHT))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::RotateRight, polarCameraAngularSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_X))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::ZoomIn, polarCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_Z))
		{
			m_pScene->GetCamera().MovePosPolar(CameraPosPolar::ZoomOut, polarCameraSpeed * dtS);
		}

		m_pScene->GetCamera().UpdateFromLookAt();
	}

	m_pTextViewFPS->SetText("FPS " + std::to_string(GetFPS()));
	m_pTextViewUPS->SetText("UPS " + std::to_string(GetUPS()));
}

void Game::OnRender(float dtS)
{
	m_pRenderer->DrawScene(*m_pScene, dtS);
}