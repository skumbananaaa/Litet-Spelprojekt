#include "..\Include\Game.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include "..\Include\Crew.h"
#include "..\Include\Grid.h"

GameObject* g_pDecalObject = nullptr;
Crew g_Crew;
Grid * g_Grid;
float g_Rot = 1.0;

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
	m_pRedMaterial(nullptr),
	m_pGreenMaterial(nullptr),
	m_pBlueMaterial(nullptr),
	m_pBoatMaterial(nullptr),
	m_pGroundMaterial(nullptr),
	m_pDecal(nullptr),
	cartesianCamera(true)
{
	m_pRenderer = new DefferedRenderer();

	m_pScene = new Scene();
	m_pTestMesh = IndexedMesh::CreateIndexedMeshFromFile("Resources/Meshes/ship.obj");
	m_pGroundTestMesh = IndexedMesh::CreateIndexedMeshFromFile("Resources/Meshes/cliff_3_low.obj");
	m_pSphereMesh = IndexedMesh::CreateIndexedMeshFromFile("Resources/Meshes/sphere.obj");

	{
		TextureParams params = {};
		params.Wrap = TEX_PARAM_REPEAT;
		params.MinFilter = TEX_LINEAR;
		params.MagFilter = TEX_LINEAR;

		m_pBloodTexture = new Texture2D("Resources/Textures/blood.png", TEX_FORMAT_RGBA, true, params);
		m_pBloodNormal = new Texture2D("Resources/Textures/bloodNormalMap.png", TEX_FORMAT_RGBA, true, params);
	}

	m_pDecal = new Decal();
	m_pDecal->SetTexture(m_pBloodTexture);
	m_pDecal->SetNormalMap(m_pBloodNormal);

	m_pBoatTexture = new Texture2D("Resources/Textures/ship.jpg", TEX_FORMAT_RGBA);
	m_pBoatNormalMap = new Texture2D("Resources/Textures/shipNormalMap.png", TEX_FORMAT_RGBA);

	m_pBoatMaterial = new Material();
	m_pBoatMaterial->SetColor(glm::vec4(0.655f, 0.639f, 0.627f, 1.0f));
	m_pBoatMaterial->SetTexture(m_pBoatTexture);
	m_pBoatMaterial->SetNormalMap(m_pBoatNormalMap);

	m_pGroundMaterial = new Material();
	m_pGroundMaterial->SetColor(glm::vec4(0.471f, 0.282f, 0.11f, 1.0f));

	m_pRedMaterial = new Material();
	m_pRedMaterial->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	m_pGreenMaterial = new Material();
	m_pGreenMaterial->SetColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	m_pBlueMaterial = new Material();
	m_pBlueMaterial->SetColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	GameObject* pGameObject = nullptr;
	pGameObject = new GameObject();
	pGameObject->SetDecal(m_pDecal);
	pGameObject->SetPosition(glm::vec3(-6.0f, 2.0f, 0.0f));
	pGameObject->SetScale(glm::vec3(3.0f, 4.0f, 3.0f));
	pGameObject->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	pGameObject->UpdateTransform();
	g_pDecalObject = pGameObject;
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(m_pBoatMaterial);
	pGameObject->SetMesh(m_pTestMesh);
	pGameObject->SetPosition(glm::vec3(0.0f, -0.8f, 0.0f));
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

	pGameObject = new GameObject();
	pGameObject->SetMaterial(m_pRedMaterial);
	pGameObject->SetMesh(m_pSphereMesh);
	pGameObject->SetPosition(glm::vec3(5.0f, 2.0f, 0.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(m_pGreenMaterial);
	pGameObject->SetMesh(m_pSphereMesh);
	pGameObject->SetPosition(glm::vec3(2.0f, 2.0f, 0.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(m_pBlueMaterial);
	pGameObject->SetMesh(m_pSphereMesh);
	pGameObject->SetPosition(glm::vec3(-5.0f, 2.0f, 0.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);1,

	g_Crew.addMember(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, -2.0f));
	m_pScene->AddGameObject(g_Crew.getMember(0));
	m_pScene->AddPointLight(g_Crew.getMember(0)->GetLight());

	g_Grid = new Grid(glm::ivec2(20, 20), glm::vec3(0.0f, 10.0f, 0.0f));

	int map[20][20]{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{1, 3, 2, 2, 2, 2, 2, 3, 3, 3, 5, 5, 2, 2, 2, 2, 2, 4, 4, 1 },
		{1, 3, 2, 2, 2, 2, 0, 3, 3, 3, 5, 5, 2, 2, 2, 2, 0, 4, 4, 1 },
		{1, 3, 2, 2, 2, 2, 2, 3, 3, 3, 5, 5, 2, 2, 2, 2, 2, 4, 4, 1 },
		{1, 3, 2, 2, 2, 2, 2, 3, 3, 3, 5, 5, 2, 2, 2, 2, 2, 4, 4, 1 },
		{1, 3, 2, 0, 2, 2, 2, 3, 0, 3, 5, 5, 2, 2, 2, 0, 2, 4, 0, 1 },
		{1, 3, 3, 3, 2, 3, 4, 4, 4, 4, 5, 5, 5, 5, 2, 5, 5, 3, 3, 1 },
		{1, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 3, 3, 1 },
		{1, 3, 3, 3, 3, 3, 4, 4, 4, 4, 0, 5, 5, 5, 5, 5, 5, 3, 3, 1 },
		{1, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 3, 1 },
		{1, 3, 2, 2, 2, 2, 2, 4, 1, 4, 4, 3, 2, 2, 2, 2, 2, 2, 0, 1 },
		{1, 3, 2, 2, 2, 2, 2, 4, 4, 4, 4, 3, 2, 2, 2, 2, 2, 2, 2, 1 },
		{1, 3, 2, 2, 2, 2, 0, 4, 4, 4, 4, 3, 2, 2, 2, 2, 2, 3, 3, 1 },
		{1, 3, 2, 2, 2, 2, 2, 4, 4, 4, 4, 3, 2, 2, 2, 2, 2, 3, 3, 1 },
		{1, 3, 2, 2, 2, 2, 2, 4, 4, 4, 4, 3, 2, 2, 2, 2, 2, 3, 3, 1 },
		{1, 3, 2, 2, 2, 0, 2, 5, 0, 5, 3, 3, 2, 2, 2, 0, 2, 4, 0, 1 },
		{1, 3, 3, 3, 2, 3, 5, 5, 5, 5, 3, 3, 3, 3, 2, 3, 4, 4, 4, 1 },
		{1, 3, 3, 3, 3, 3, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 4, 4, 4, 1 },
		{1, 3, 3, 3, 3, 3, 5, 5, 5, 5, 3, 3, 3, 3, 3, 3, 4, 4, 4, 1 },
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};

	for (int i = 0; i < g_Grid->GetSize().x; i++)
	{
		for (int j = 0; j < g_Grid->GetSize().y; j++)
		{
			g_Grid->GetTile(glm::ivec2(i, j))->SetID(map[i][j]);
			g_Grid->SetColor(glm::ivec2(i, j), glm::vec4(map[i][j] / 10.0f, map[i][j] / 10.0f, 0.0f, 1.0f));
			m_pScene->AddGameObject(g_Grid->GetTile(glm::ivec2(i, j)));
		}
	}

	Camera* pCamera = new Camera(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	float aspect = static_cast<float>(GetWindow().GetWidth()) / static_cast<float>(GetWindow().GetHeight());
	pCamera->CreatePerspective(glm::radians<float>(90.0f), aspect, 0.01f, 100.0f);
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

	//Lights
	DirectionalLight* pDirectionalLight = new DirectionalLight(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec3(0.0f, 0.5f, 0.5f));
	m_pScene->AddDirectionalLight(pDirectionalLight);

	m_pScene->AddPointLight(new PointLight(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::vec3(5.0f, 2.0f, 0.0f)));
	m_pScene->AddPointLight(new PointLight(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec3(2.0f, 2.0f, 0.0f)));
	m_pScene->AddPointLight(new PointLight(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec3(-5.0f, 2.0f, 0.0f)));

	m_pTextViewFPS = new TextView(0, 720, 200, 50, "FPS");
	m_pTextViewUPS = new TextView(0, 690, 200, 50, "UPS");

	GetGUIManager().AddGUIObject(m_pTextViewFPS);
	GetGUIManager().AddGUIObject(m_pTextViewUPS);

	//Audio
	m_pSoundEffect = new SoundEffect("Resources/Audio/fart2.wav");
	m_pTestAudioSource = new AudioSource(*m_pSoundEffect);
	m_pTestAudioSource->SetPitch(2.0f);
	m_pTestAudioSource->Play();

	AudioListener::SetPosition(glm::vec3(0.0f));
}

Game::~Game()
{
	Delete(m_pBloodTexture);
	Delete(m_pBloodNormal);

	Delete(m_pFontRenderer);
	Delete(m_pRenderer);
	
	Delete(m_pScene);
	
	Delete(m_pSphereMesh);
	Delete(m_pTestMesh);
	Delete(m_pWaterMesh);
	Delete(m_pGroundTestMesh);
	
	Delete(m_pBoatTexture);
	Delete(m_pBoatNormalMap);
	
	Delete(m_pDecal);
	Delete(m_pRedMaterial);
	Delete(m_pGreenMaterial);
	Delete(m_pBlueMaterial);
	Delete(m_pBoatMaterial);
	Delete(m_pGroundMaterial);
	
	Delete(m_pTextViewFPS);
	Delete(m_pTextViewUPS);
	
	Delete(m_pSoundEffect);
	
	Delete(m_pTestAudioSource);

	Delete(g_Grid);
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

	AudioListener::SetPosition(m_pScene->GetCamera().GetPosition());

	static float decalRot = 0.0f;
	static float decalX = g_pDecalObject->GetPosition().x;
	static float decalXSpeed = -1.0f;
	
	if (decalX > 6.5f)
	{
		decalXSpeed = -1.0f;
	}
	else if (decalX < -6.5f)
	{
		decalXSpeed = 1.0f;
	}

	decalX += decalXSpeed * dtS;
	decalRot += (glm::half_pi<float>() / 2.0f) * dtS;
	Crewmember * derp = g_Crew.getMember(0);
	derp->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, decalRot));
	derp->UpdateTransform();
	g_pDecalObject->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, decalRot));
	g_pDecalObject->SetPosition(glm::vec3(decalX, 0.0f, 0.0f));
	g_pDecalObject->UpdateTransform();
}

void Game::OnRender(float dtS)
{
	m_pRenderer->DrawScene(*m_pScene, dtS);
}