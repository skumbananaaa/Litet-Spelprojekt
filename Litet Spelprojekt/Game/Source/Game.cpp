#include "..\Include\Game.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <World/Grid.h>


#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/Button.h>
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Slider.h>
#include <Graphics/GUI/PanelScrollable.h>

#if defined(_DEBUG)
//#define DRAW_DEBUG_BOXES
#endif

#define CHOSEN_LIGHT glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define DEFAULT_LIGHT glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)

GameObject* g_pDecalObject = nullptr;

float g_Rot = 1.0;

Game::Game() noexcept : 
	Application(false, 1920, 1080, "", true),
	m_pRenderer(nullptr),
	m_pDebugRenderer(nullptr),
	m_pScene(nullptr),
	m_pSkyBoxTex(nullptr),
	m_pWorld(nullptr),
	m_pTextViewFPS(nullptr),
	m_pTextViewUPS(nullptr),
	m_pTestAudioSource(nullptr),
	cartesianCamera(false),
	m_CurrentElevation(2)
{
	m_pTextViewFPS = new TextView(0, GetWindow().GetHeight() - 60, 200, 50, "FPS");
	m_pTextViewUPS = new TextView(0, GetWindow().GetHeight() - 80, 200, 50, "UPS");
	m_pTextViewCrew = new TextView(0, 0, GetWindow().GetWidth(), 50, "Crew: ");
	m_pTextViewFile = new TextView((GetWindow().GetWidth() - 300) / 2, (GetWindow().GetHeight() - 50) / 2 + 50, 300, 50, "Loading...");
	m_pLoadingBar = new ProgressBar((GetWindow().GetWidth() - 300) / 2, (GetWindow().GetHeight() - 50) / 2, 300, 50);

	GetGUIManager().Add(m_pTextViewFPS);
	GetGUIManager().Add(m_pTextViewUPS);
	GetGUIManager().Add(m_pTextViewCrew);
	GetGUIManager().Add(m_pTextViewFile);
	GetGUIManager().Add(m_pLoadingBar);
}

Game::~Game()
{
	DeleteSafe(m_pRenderer);
	DeleteSafe(m_pDebugRenderer);

	DeleteSafe(m_pSkyBoxTex);

	DeleteSafe(m_pScene);
	DeleteSafe(m_pScene2);

	DeleteSafe(m_pTextViewFPS);
	DeleteSafe(m_pTextViewUPS);
	DeleteSafe(m_pTextViewCrew);
	DeleteSafe(m_pTextViewFile);
	DeleteSafe(m_pLoadingBar);
	
	DeleteSafe(m_pTestAudioSource);
	DeleteSafe(m_pWorld);
}

void Game::OnResourceLoading(const std::string& file, float percentage)
{
	m_pTextViewFile->SetText("Loading: " + file);
	m_pLoadingBar->SetPercentage(percentage);
}

void Game::OnResourcesLoaded()
{
	GetGUIManager().Remove(m_pTextViewFile);
	GetGUIManager().Remove(m_pLoadingBar);

	m_pScene = new Scene();

	Camera* pCamera = new Camera(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	float aspect = static_cast<float>(GetWindow().GetWidth()) / static_cast<float>(GetWindow().GetHeight());
	pCamera->CreatePerspective(glm::radians<float>(90.0f), aspect, 0.1f, 1000.0f);
	pCamera->UpdateFromPitchYaw();
	m_pScene->SetCamera(pCamera);


	AudioListener::SetPosition(glm::vec3(0.0f));

	GameObject* pGameObject = nullptr;
	{
		pGameObject = new GameObject();
		pGameObject->SetDecal(DECAL::BLOOD);
		pGameObject->SetPosition(glm::vec3(-6.0f, 2.0f, 0.0f));
		pGameObject->SetScale(glm::vec3(3.0f, 4.0f, 3.0f));
		pGameObject->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		pGameObject->UpdateTransform();
		g_pDecalObject = pGameObject;
		m_pScene->AddGameObject(pGameObject);
	}
	/*{
		pGameObject = new GameObject();
		pGameObject->SetName("ship");
		pGameObject->SetMaterial(MATERIAL::GROUND);
		pGameObject->SetMesh(MESH::SHIP);
		pGameObject->SetPosition(glm::vec3(5.5f, -3.0f, 12.5f));
		pGameObject->SetScale(glm::vec3(1.0f));
		pGameObject->UpdateTransform();
		m_pScene->AddGameObject(pGameObject);
	}*/
	{
		pGameObject = new GameObject();
		pGameObject->SetMaterial(MATERIAL::GROUND);
		pGameObject->SetMesh(MESH::CLIFF_3_LOW);
		pGameObject->SetPosition(glm::vec3(0.0f, -1.4f, 0.0f));
		pGameObject->SetScale(glm::vec3(0.4f));
		pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::half_pi<float>()));
		pGameObject->UpdateTransform();
		m_pScene->AddGameObject(pGameObject);
	}
	{
		pGameObject = new GameObject();
		pGameObject->SetMaterial(MATERIAL::RED);
		pGameObject->SetMesh(MESH::CUBE_OBJ);
		pGameObject->SetPosition(glm::vec3(5.5f, 0.0f, 20.5f));
		pGameObject->SetScale(glm::vec3(10.0f, 0.1f, 40.0f));
		pGameObject->UpdateTransform();
		m_pScene->AddGameObject(pGameObject);
	}
	{
		pGameObject = new GameObject();
		pGameObject->SetMaterial(MATERIAL::GREEN);
		pGameObject->SetMesh(MESH::CUBE_OBJ);
		pGameObject->SetPosition(glm::vec3(5.5f, 2.0f, 20.5f));
		pGameObject->SetScale(glm::vec3(10.0f, 0.1f, 40.0f));
		pGameObject->UpdateTransform();
		m_pScene->AddGameObject(pGameObject);
	}
	{
		pGameObject = new GameObject();
		pGameObject->SetMaterial(MATERIAL::BLUE);
		pGameObject->SetMesh(MESH::CUBE_OBJ);
		pGameObject->SetPosition(glm::vec3(5.5f, 4.0f, 20.5f));
		pGameObject->SetScale(glm::vec3(10.0f, 0.1f, 40.0f));
		pGameObject->UpdateTransform();
		m_pScene->AddGameObject(pGameObject);
	}
	{
		//Water?? YAAAS
		pGameObject = new GameObject();
		pGameObject->SetIsReflectable(true);
		pGameObject->SetMesh(MESH::QUAD);
		pGameObject->SetScale(glm::vec3(200.0f));
		pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::half_pi<float>()));
		pGameObject->UpdateTransform();
		m_pScene->AddGameObject(pGameObject);
	}

	pGameObject = new GameObject();
	pGameObject->SetName("ship");
	pGameObject->SetMaterial(MATERIAL::BOAT);
	pGameObject->SetMesh(MESH::SHIP);
	pGameObject->SetPosition(glm::vec3(5.5f, -3.0f, 12.5f));
	pGameObject->SetScale(glm::vec3(1.0f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);
	
	//test objects
	pGameObject = ResourceHandler::CreateGameObject(GAMEOBJECT::BED_SINGLE);
	pGameObject->SetPosition(glm::vec3(-5.0f, 2.0f, -10.0f));
	pGameObject->SetScale(glm::vec3(1.0f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = ResourceHandler::CreateGameObject(GAMEOBJECT::BED_BUNK);
	pGameObject->SetPosition(glm::vec3(-5.0f, 4.0f, -10.0f));
	pGameObject->SetScale(glm::vec3(1.0f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);
	
	m_pWorld = WorldSerializer::Read("world.json");

	int gameObjects = m_pWorld->GetNumWorldObjects();
	
	for (int i = 0; i < gameObjects; i++)
	{
		WorldObject worldObject = m_pWorld->GetWorldObject(i);
		int32 width = m_pWorld->GetLevel(worldObject.TileId.y)->GetSizeX();
		int32 height = m_pWorld->GetLevel(worldObject.TileId.y)->GetSizeZ();
		int floorLevel = worldObject.TileId.y / 2;
		GameObject* pGameObject = ResourceHandler::CreateGameObject(worldObject.GameObject);
		glm::vec3 pos = worldObject.TileId;
		pos.x += 1;
		pos.z += 1;
		pGameObject->SetPosition(pos);
		pGameObject->SetRotation(glm::vec4(0, 1, 0, worldObject.Rotation));
		m_pScene->AddGameObject(pGameObject);
	}

	uint32 wallMaterials[] = {
		MATERIAL::BLUE,
		MATERIAL::RED,
		MATERIAL::GREEN
	};

	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::BLUE);
	pGameObject->SetMesh(MESH::CUBE_INV_NORMALS);
	pGameObject->SetPosition(pCamera->GetLookAt());
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	pGameObject->SetName("cameraLookAt");
	m_pScene->AddGameObject(pGameObject);

	//Water?? YAAAS
	pGameObject = new GameObject();
	pGameObject->SetIsReflectable(true);
	pGameObject->SetMesh(MESH::QUAD);
	pGameObject->SetMaterial(MATERIAL::WATER);
	pGameObject->SetScale(glm::vec3(200.0f));
	pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::half_pi<float>()));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	PlanarReflector* pReflector = new PlanarReflector(glm::vec3(0.0f, 1.0f, 0.0f), 0.01f);
	m_pScene->AddPlanarReflector(pReflector);
	((WaterMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER))->SetPlanarReflector(pReflector);

	m_pWorld = WorldSerializer::Read("world.json");

	//Enable clipplane for wallmaterial
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetCullMode(CULL_MODE_NONE);

	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetCullMode(CULL_MODE_NONE);
	((WallMaterial*)ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD))->SetDissolveFactor(1.0f);

	SetClipPlanes();

	// Generate walls
	for (int level = 0; level < m_pWorld->GetNumLevels(); level += 2) 
	{
		m_pWorld->GenerateWalls(level);
		glm::vec4 wall;

		for (int i = 0; i < m_pWorld->GetLevel(level)->GetNrOfWalls(); i++)
		{
			wall = m_pWorld->GetLevel(level)->GetWall(i);
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::WALL_STANDARD);
			pGameObject->SetMesh(MESH::CUBE);
			pGameObject->SetPosition(glm::vec3(wall.x, 1.0f + level, wall.y));
			pGameObject->SetScale(glm::vec3(wall.z + 0.1f, 2.0f, wall.w + 0.1f));
			pGameObject->UpdateTransform();
			
			m_pScene->AddGameObject(pGameObject);
		}
	}

	std::string names[] = {
		"Granfeldt",
		"Ola",
		"Sven",
		"Gunnar",
		"Fysik-GW",
		"Robban",
		"Bengan",
		"Ragnar",
		"Klasse",
		"Gustafsson",
		"Nisse",
		"Per-Egon",
		"Knut",
		"Britt-Marie",
		"Bert Karlsson"
	};

	float x, y, z;
	for (int i = 0; i < NUM_CREW; i++)
	{
		y = (std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2;
		x = std::rand() % (m_pWorld->GetLevel(y)->GetSizeX() - 2) + 1;
		z = std::rand() % (m_pWorld->GetLevel(y)->GetSizeZ() - 2) + 1;
		m_Crew.AddMember(DEFAULT_LIGHT, glm::vec3(x, 0.9f + y, z), 100, names[i % NUM_CREW]);
		m_CrewList[i] = "";
		m_pScene->AddGameObject(m_Crew.GetMember(i));
		m_pScene->AddSpotLight(m_Crew.GetMember(i)->GetTorch());
		m_pScene->AddPointLight(m_Crew.GetMember(i)->GetLight());
		m_Crew.GetMember(i)->SetPath(m_pWorld);
		m_Crew.GetMember(i)->UpdateTransform();
	}


	m_pRenderer = new DefferedRenderer();
	//m_pRenderer = new OrthographicRenderer();
	m_pDebugRenderer = new DebugRenderer();

	m_pSkyBoxTex = new TextureCube(ResourceHandler::GetTexture2D(TEXTURE::HDR));
	m_pScene->SetSkyBox(new SkyBox(m_pSkyBoxTex));

	//Lights
	DirectionalLight* pDirectionalLight = new DirectionalLight(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec3(0.0f, 0.5f, 0.5f));
	m_pScene->AddDirectionalLight(pDirectionalLight);

	m_pScene->AddPointLight(new PointLight(glm::vec3(5.0f, 2.0f, -10.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	m_pScene->AddPointLight(new PointLight(glm::vec3(2.0f, 2.0f, -10.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	m_pScene->AddPointLight(new PointLight(glm::vec3(-5.0f, 2.0f, -10.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));

	//m_pScene->AddPointLight(new PointLight(glm::vec3(2.0f, 3.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	m_pScene->AddSpotLight(new SpotLight(glm::vec3(6.0f, 5.9f, 10.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.5f)), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	m_pScene->AddSpotLight(new SpotLight(glm::vec3(6.0f, 5.9f, 25.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.5f)), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	

		//Audio
	m_pTestAudioSource = AudioSource::CreateMusicSource(MUSIC::WAVES_AND_SEAGULLS);
	m_pTestAudioSource->SetPitch(1.0f);
	m_pTestAudioSource->SetLooping(true);
	m_pTestAudioSource->Play();

	m_pScene2 = new Scene();

	pCamera = new Camera(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	pCamera->CreatePerspective(glm::radians<float>(90.0f), GetWindow().GetAspectRatio(), 0.1f, 1000.0f);
	pCamera->UpdateFromPitchYaw();
	m_pScene2->SetCamera(pCamera);

	m_pScene2->AddDirectionalLight(new DirectionalLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	
	for (uint32 i = 0; i < 5; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(MESH::SPHERE);
		pGameObject->SetMaterial(MATERIAL::RED);
		pGameObject->SetPosition(glm::vec3(0.0f, 0.0f, i * 2.0f));
		pGameObject->UpdateTransform();
		m_pScene2->AddGameObject(pGameObject);
	}

	for (uint32 i = 0; i < 5; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(MESH::SPHERE);
		pGameObject->SetMaterial(MATERIAL::RED_1);
		pGameObject->SetPosition(glm::vec3(2.0f, 0.0f, i * 2.0f));
		pGameObject->UpdateTransform();
		m_pScene2->AddGameObject(pGameObject);
	}

	for (uint32 i = 0; i < 5; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(MESH::SPHERE);
		pGameObject->SetMaterial(MATERIAL::RED_2);
		pGameObject->SetPosition(glm::vec3(4.0f, 0.0f, i * 2.0f));
		pGameObject->UpdateTransform();
		m_pScene2->AddGameObject(pGameObject);
	}

	for (uint32 i = 0; i < 5; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(MESH::SPHERE);
		pGameObject->SetMaterial(MATERIAL::RED_3);
		pGameObject->SetPosition(glm::vec3(6.0f, 0.0f, i * 2.0f));
		pGameObject->UpdateTransform();
		m_pScene2->AddGameObject(pGameObject);
	}

	for (uint32 i = 0; i < 5; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(MESH::SPHERE);
		pGameObject->SetMaterial(MATERIAL::RED_4);
		pGameObject->SetPosition(glm::vec3(8.0f, 0.0f, i * 2.0f));
		pGameObject->UpdateTransform();
		m_pScene2->AddGameObject(pGameObject);
	}

	pGameObject = new GameObject();
	pGameObject->SetMesh(MESH::QUAD);
	pGameObject->SetIsReflectable(true);
	pGameObject->SetMaterial(MATERIAL::WATER);
	pGameObject->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::radians<float>(90.0f)));
	pGameObject->SetScale(glm::vec3(30.0f));
	pGameObject->UpdateTransform();
	m_pScene2->AddGameObject(pGameObject);

	pReflector = new PlanarReflector(glm::vec3(0.0f, 1.0f, 0.0f), 0.01f);
	m_pScene2->AddPlanarReflector(pReflector);

	//((WaterMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER))->SetPlanarReflector(pReflector);

	m_pCurrentScene = m_pScene;
}

void Game::OnUpdateLoading(float dtS)
{
	m_pTextViewFPS->SetText("FPS " + std::to_string(GetFPS()));
	m_pTextViewUPS->SetText("UPS " + std::to_string(GetUPS()));
}

void Game::OnRenderLoading(float dtS)
{
}

void Game::OnKeyUp(KEY keycode)
{
	switch (keycode)
	{
		case KEY_SPACE:
		{
			m_pScene->ExtendScene(false);
			break;
		}
	}

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
		case KEY_P:
		{
			m_pTestAudioSource->TogglePause();
			break;
		}
		case KEY_SPACE:
		{
			m_pScene->ExtendScene(true);
			break;
		}
		case KEY_L:
		{
			for (int i = 0; i < m_Crew.GetCount(); i++)
			{
				m_Crew.GetMember(i)->SwitchLight();
			}
			break;
		}
	}

	Application::OnKeyDown(keycode);
}

void Game::OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	if (/*Input::IsKeyDown(KEY_LEFT_ALT) &&*/ !cartesianCamera)
	{
		if (Input::IsButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
		{
			const float cameraRotationSensitivity = 0.005f;
			glm::vec2 deltaPosition = cameraRotationSensitivity * (position - lastPosition);

			m_pCurrentScene->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateX, deltaPosition.x);
			m_pCurrentScene->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateY, -deltaPosition.y);
		}
		
		if (Input::IsButtonDown(MouseButton::MOUSE_BUTTON_RIGHT))
		{
			const float cameraMoveSensitivityX = 0.5f;
			const float cameraMoveSensitivityY = 0.025f;
			glm::vec2 deltaPosition = cameraMoveSensitivityY * (position - lastPosition);
			glm::vec3 forward(0.0f);
			forward.x = m_pCurrentScene->GetCamera().GetFront().x;
			forward.z = m_pCurrentScene->GetCamera().GetFront().z;
			m_pCurrentScene->GetCamera().MoveWorldCoords(-forward * deltaPosition.y, true);
			m_pCurrentScene->GetCamera().MoveLocalCoords(glm::vec3(cameraMoveSensitivityX * deltaPosition.x, 0.0f, 0.0f), true);

		}
	}
}

void Game::OnMouseReleased(MouseButton mousebutton, const glm::vec2 & position)
{
	switch (mousebutton)
	{
		case MOUSE_BUTTON_LEFT:
		{
			PickPosition();
			break;
		}
		case MOUSE_BUTTON_RIGHT:
		{
			PickCrew();
			break;
		}
	}
}

void Game::OnMouseScroll(const glm::vec2& offset, const glm::vec2& position)
{
	if (!cartesianCamera)
	{
		if (Input::IsKeyDown(KEY_LEFT_ALT))
		{
			if (offset.y > 0.0f)
			{
				m_pCurrentScene->GetCamera().MoveWorldCoords(glm::vec3(0.0f, 1.0f, 0.0f), true);
			}
			else
			{
				m_pCurrentScene->GetCamera().MoveWorldCoords(glm::vec3(0.0f, -1.0f, 0.0f), true);
			}

			SetClipPlanes();
		}
		{
			const float cameraZoomSensitivity = 0.1f;
			m_pCurrentScene->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::Zoom, cameraZoomSensitivity * offset.y);
		}
	}
}

void Game::OnUpdate(float dtS)
{
	static float dist = 0.0f;
	dist += 0.02f * dtS;
	((WaterMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER))->SetDistortionFactor(dist);

	m_pCurrentScene->OnUpdate(dtS);

	float cartesianCameraSpeed = 5.0F;
	float cartesianCameraAngularSpeed = 1.5F;

	if (cartesianCamera)
	{

		glm::vec3 localMove(0.0f);

		if (Input::IsKeyDown(KEY_W))
		{
			localMove.z = cartesianCameraSpeed * dtS;
		}
		else if (Input::IsKeyDown(KEY_S))
		{
			localMove.z = -cartesianCameraSpeed * dtS;
		}

		if (Input::IsKeyDown(KEY_A))
		{
			localMove.x = cartesianCameraSpeed * dtS;
		}
		else if (Input::IsKeyDown(KEY_D))
		{
			localMove.x = -cartesianCameraSpeed * dtS;
		}

		if (Input::IsKeyDown(KEY_E))
		{
			localMove.y = cartesianCameraSpeed * dtS;
		}
		else if (Input::IsKeyDown(KEY_Q))
		{
			localMove.y = -cartesianCameraSpeed * dtS;
		}

		m_pScene->GetCamera().MoveLocalCoords(localMove);

		if (Input::IsKeyDown(KEY_UP))
		{
			m_pCurrentScene->GetCamera().OffsetPitch(cartesianCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_DOWN))
		{
			m_pCurrentScene->GetCamera().OffsetPitch(-cartesianCameraAngularSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_LEFT))
		{
			m_pCurrentScene->GetCamera().OffsetYaw(-cartesianCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_RIGHT))
		{
			m_pCurrentScene->GetCamera().OffsetYaw(cartesianCameraAngularSpeed * dtS);
		}

		m_pCurrentScene->GetCamera().UpdateFromPitchYaw();
	}
	else
	{
		//Polar
		/*static float polarCameraSpeed = 5.0f;
		static float polarCameraAngularSpeed = 0.8f;

		if (Input::IsKeyDown(KEY_W))
		{
			glm::vec3 forward(0.0f);
			forward.x = m_pScene->GetCamera().GetFront().x;
			forward.z = m_pScene->GetCamera().GetFront().z;
			m_pScene->GetCamera().MoveWorldCoords(forward * polarCameraSpeed * dtS, true);
		}
		else if (Input::IsKeyDown(KEY_S))
		{
			glm::vec3 forward(0.0f);
			forward.x = m_pScene->GetCamera().GetFront().x;
			forward.z = m_pScene->GetCamera().GetFront().z;
			m_pScene->GetCamera().MoveWorldCoords(-forward * polarCameraSpeed * dtS, true);
		}

		if (Input::IsKeyDown(KEY_A))
		{
			m_pScene->GetCamera().MoveLocalCoords(glm::vec3(polarCameraSpeed * dtS, 0.0f, 0.0f), true);
		}
		else
		{
			m_pScene->GetCamera().MoveLocalCoords(glm::vec3(-polarCameraSpeed * dtS, 0.0f, 0.0f), true);
		}

		if (Input::IsKeyDown(KEY_E))
		{
			m_pScene->GetCamera().MoveWorldCoords(glm::vec3(0.0f, polarCameraSpeed * dtS, 0.0f), true);
		}
		else if (Input::IsKeyDown(KEY_Q))
		{
			m_pScene->GetCamera().MoveWorldCoords(glm::vec3(0.0f, -polarCameraSpeed * dtS, 0.0f), true);
		}

		if (Input::IsKeyDown(KEY_UP))
		{
			m_pScene->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateY, polarCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_DOWN))
		{
			m_pScene->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateY, -polarCameraAngularSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_LEFT))
		{
			m_pScene->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateX, polarCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_RIGHT))
		{
			m_pScene->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateX, -polarCameraAngularSpeed * dtS);
		}

		AudioListener::SetPosition(m_pScene->GetCamera().GetPosition());
		AudioListener::SetOrientation(m_pScene->GetCamera().GetFront(), m_pScene->GetCamera().GetUp());

		static float decalRot = 0.0f;
		static float decalX = g_pDecalObject->GetPosition().x;
		static float decalXSpeed = -1.0f;

		if (decalX > 6.5f)
		{
			m_pScene->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::Zoom, polarCameraSpeed * dtS);
		}
		else if (decalX < -6.5f)
		{
			m_pScene->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::Zoom, -polarCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_E))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Up, polarCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_Q))
		{
			m_pScene->GetCamera().MoveLookAtAndPosPolar(CameraDirCartesian::Down, polarCameraSpeed * dtS);
		}*/

		m_pCurrentScene->GetCamera().UpdateFromLookAt();
	}

	GameObject* pCameraLookAt = m_pCurrentScene->GetGameObject("cameraLookAt");
	if (pCameraLookAt)
	{
		pCameraLookAt->SetPosition(m_pCurrentScene->GetCamera().GetLookAt());
	}

	m_pTextViewFPS->SetText("FPS " + std::to_string(GetFPS()));
	m_pTextViewUPS->SetText("UPS " + std::to_string(GetUPS()));

	AudioListener::SetPosition(m_pScene->GetCamera().GetPosition());
	AudioListener::SetOrientation(m_pScene->GetCamera().GetFront(), m_pScene->GetCamera().GetUp());

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
	//decalRot += (glm::half_pi<float>() / 2.0f) * dtS;

	g_pDecalObject->SetRotation(glm::vec4(0.0f, 0.0f, 1.0f, glm::radians<float>(-45.0f)));
	g_pDecalObject->SetPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	g_pDecalObject->UpdateTransform();
	if (Input::IsKeyPressed(KEY_NUMPAD_2))
	{
		if (m_CurrentElevation > 0)
		{
			m_CurrentElevation--;
		}
		
		SetClipPlanes();
		std::cout << "Elevation: " << m_CurrentElevation << std::endl;
	}

	if (Input::IsKeyPressed(KEY_NUMPAD_8))
	{
		if (m_CurrentElevation < 2)
		{
			m_CurrentElevation++;
		}

		SetClipPlanes(); 
		std::cout << "Elevation: " << m_CurrentElevation << std::endl;
	}
}

void Game::OnRender(float dtS)
{
	m_pRenderer->DrawScene(*m_pCurrentScene, dtS);

#if defined(DRAW_DEBUG_BOXES)
	m_pDebugRenderer->DrawScene(*m_pCurrentScene);
#endif
	
}

void Game::PickPosition() {
	glm::vec3 rayDir = GetRay(Input::GetMousePosition(), GetWindow().GetWidth(), GetWindow().GetHeight());
	glm::vec3 rayOrigin = m_pScene->GetCamera().GetPosition();
	glm::vec3 pointOnSurface = glm::vec3(0.0f, 0.0f, 0.0f);
	
	glm::vec3 normal(0.0f, 1.0f, 0.0f);

	float t = -1, lastT = -1;
	for (int d = m_pWorld->GetNumLevels() - 2; d >= 0; d -= 2)
	{
		if (glm::dot(normal, rayDir) < -0.01)
		{
			t = (d - glm::dot(normal, rayOrigin)) / glm::dot(normal, rayDir);
		}

		if ((t >= 0 && lastT == -1) || (t > 0 && t < lastT))
		{
			pointOnSurface = rayOrigin + rayDir * t;
			bool extended = m_pScene->IsExtended();
			if (pointOnSurface.x > 5 * d * extended)
			{
				lastT = t;
			}
			else
			{
				pointOnSurface = glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}
	}	

	if (pointOnSurface != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		for (int i = 0; i < m_Crew.GetCount(); i++)
		{
			if (m_Crew.GetMember(i)->GetLight()->GetColor() == CHOSEN_LIGHT || m_Crew.GetMember(i)->GetTorch()->GetColor() == CHOSEN_LIGHT)
			{
				m_Crew.GetMember(i)->FindPath(glm::round(pointOnSurface));
			}
		}
	}
}

void Game::PickCrew()
{
	glm::vec3 rayDir = GetRay(Input::GetMousePosition(), GetWindow().GetWidth(), GetWindow().GetHeight());
	glm::vec3 rayOrigin = m_pScene->GetCamera().GetPosition();

	float lastT = -1;
	uint32 id = -1;

	for (int i = 0; i < m_Crew.GetCount(); i++)
	{
		glm::vec3 centre = m_Crew.GetMember(i)->GetPosition();

		glm::vec3 normals[]{
			m_Crew.GetMember(i)->GetDirection(),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::normalize(glm::cross(normals[0], normals[1]))
		};

		float h[] = {
			0.1,
			0.9,
			0.25
		};

		float d1[] = {
			glm::dot(centre - normals[0] * h[0], normals[0]),
			glm::dot(centre - normals[1] * h[1], normals[1]),
			glm::dot(centre - normals[2] * h[2], normals[2])
		};
		float d2[] = {
			glm::dot(centre + normals[0] * h[0], normals[0]),
			glm::dot(centre + normals[1] * h[1], normals[1]),
			glm::dot(centre + normals[2] * h[2], normals[2])
		};

		float t1[3];
		float t2[3];
		float t_min[3];
		float t_max[3];

		float t = -1;
		float min_t, max_t;

		for (int j = 0; j < 3; j++)
		{
			if (std::abs(glm::dot(normals[j], rayDir)) > 0.01)
			{
				t1[j] = (d1[j] - glm::dot(normals[j], rayOrigin)) / glm::dot(normals[j], rayDir);
				t2[j] = (d2[j] - glm::dot(normals[j], rayOrigin)) / glm::dot(normals[j], rayDir);

				t_min[j] = std::min(t1[j], t2[j]);
				t_max[j] = std::max(t1[j], t2[j]);
			}
			else if (-glm::dot(normals[0], centre - rayOrigin) - h[j] > 0 || -glm::dot(normals[0], centre - rayOrigin) + h[j] < 0)
				return;
		}

		min_t = std::max(t_min[0], t_min[1]);
		min_t = std::max(min_t, t_min[2]);
		max_t = std::min(t_max[0], t_max[1]);
		max_t = std::min(max_t, t_max[2]);

		if (min_t <= max_t && max_t >= 0)
		{
			if (t_min > 0)
				t = min_t;
			else
				t = max_t;
		}

		if (t > 0 && lastT == -1 || t >= 0 && t < lastT)
		{
			lastT = t;
			id = i;
		}
	}

	if (id != -1)
	{
		if (m_Crew.GetMember(id)->GetLight()->GetColor() == CHOSEN_LIGHT)
		{
			m_Crew.GetMember(id)->GetLight()->SetColor(DEFAULT_LIGHT);
			m_CrewList[id] = "";
		}
		else if (m_Crew.GetMember(id)->GetTorch()->GetColor() == CHOSEN_LIGHT)
		{
			m_Crew.GetMember(id)->GetTorch()->SetColor(DEFAULT_LIGHT);
			m_CrewList[id] = "";
		}
		else if (m_Crew.GetMember(id)->GetLight()->GetColor() == DEFAULT_LIGHT)
		{
			m_Crew.GetMember(id)->GetLight()->SetColor(CHOSEN_LIGHT);
			m_CrewList[id] = m_Crew.GetMember(id)->GetName() + " | ";
		}
		else if (m_Crew.GetMember(id)->GetTorch()->GetColor() == DEFAULT_LIGHT)
		{
			m_Crew.GetMember(id)->GetTorch()->SetColor(CHOSEN_LIGHT);
			m_CrewList[id] = m_Crew.GetMember(id)->GetName() + " | ";
		}
		std::string crewList = "";
		for (int i = 0; i < NUM_CREW; i++)
		{
			crewList += m_CrewList[i];
		}
		m_pTextViewCrew->SetText("Crew: " + crewList);
	}
}

glm::vec3 Game::GetRay(const glm::vec2 & mousepos, uint32 windowWidth, uint32 windowHeight)
{
	glm::vec4 rayDir4((2.0f * mousepos.x) / windowWidth - 1.0f, 1.0f - (2.0f * mousepos.y) / windowHeight, -1.0, 1.0);
	rayDir4 = m_pScene->GetCamera().GetInverseProjectionMatrix() * rayDir4;
	rayDir4 = glm::vec4(glm::vec2(rayDir4), -1.0, 0.0);
	rayDir4 = m_pScene->GetCamera().GetInverseViewMatrix() * rayDir4;

	glm::vec3 rayDir = glm::normalize(glm::vec3(rayDir4));

	return rayDir;
}


void Game::SetClipPlanes()
{
	/*ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 1.8f + (m_CurrentElevation * 2.0f));
	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 2.0f + (m_CurrentElevation * 2.0f));
	ResourceHandler::GetMaterial(MATERIAL::CREW_STANDARD)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 2.0f + (m_CurrentElevation * 2.0f));*/
	
	float elevation = glm::clamp((glm::floor(m_pScene->GetCamera().GetLookAt().y / 2.0f)), 0.0f, 2.0f);
	((WallMaterial*)ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD))->SetClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 1.99f + (elevation * 2.0f)), 1);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetLevelClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 1.99f + (elevation * 2.0f)));
	
	
	//m_pRenderer->SetClipDistance(glm::vec4(0.0f, -1.0f, 0.0f, 1.99f + (elevation * 2.0f)), 1);
	//m_pRenderer->SetClipDistance(glm::vec4(0.0f, -1.0f, 0.0f, 1.99f + (elevation * 2.0f)), 1);
}
