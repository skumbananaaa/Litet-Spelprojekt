#include "..\Include\Game.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <World/Grid.h>
#include "..\Include\Crew.h"
#include "..\Include\Path.h"

#if defined(_DEBUG)
//#define DRAW_DEBUG_BOXES
#endif

GameObject* g_pDecalObject = nullptr;
Crew g_Crew;
//Grid* g_Grid;

float g_Rot = 1.0;

Game::Game() noexcept 
	: Application(false),
	m_pRenderer(nullptr),
	m_pDebugRenderer(nullptr),
	m_pScene(nullptr),
	m_pSkyBoxTex(nullptr),
	m_pWorld(nullptr),
	m_pSoundEffect(nullptr),
	m_pTextViewFPS(nullptr),
	m_pTextViewUPS(nullptr),
	m_pMusic(nullptr),
	m_pTestAudioSource(nullptr),
	cartesianCamera(true),
	m_CurrentElevation(2)
{
	m_pScene = new Scene();
	ResourceHandler::LoadResources(this);

	m_pRenderer = new DefferedRenderer();
	m_pDebugRenderer = new DebugRenderer();

	//const void * paths[6];
	const char* paths[6];
	paths[0] = "Resources/Textures/SkyBoxTextures/ss_ft.png"; //forward
	paths[1] = "Resources/Textures/SkyBoxTextures/ss_bk.png"; //back
	paths[2] = "Resources/Textures/SkyBoxTextures/ss_up.png"; //up
	paths[3] = "Resources/Textures/SkyBoxTextures/ss_dn.png"; //down
	paths[4] = "Resources/Textures/SkyBoxTextures/ss_rt.png"; //right
	paths[5] = "Resources/Textures/SkyBoxTextures/ss_lf.png"; //left

	TextureParams cubeParams = {};
	cubeParams.Wrap = TEX_PARAM_EDGECLAMP;
	cubeParams.MagFilter = TEX_PARAM_LINEAR;
	cubeParams.MinFilter = TEX_PARAM_LINEAR;
	//m_pSkyBoxTex = new TextureCube(paths, TEX_FORMAT_RGBA, cubeParams);
	m_pSkyBoxTex = new TextureCube(ResourceHandler::GetTexture2D(TEXTURE::HDR));
	m_pScene->SetSkyBox(new SkyBox(m_pSkyBoxTex));
	Camera* pCamera = new Camera(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	float aspect = static_cast<float>(GetWindow().GetWidth()) / static_cast<float>(GetWindow().GetHeight());
	pCamera->CreatePerspective(glm::radians<float>(90.0f), aspect, 0.1f, 1000.0f);
	pCamera->UpdateFromPitchYaw();
	m_pScene->SetCamera(pCamera);

	//Lights
	DirectionalLight* pDirectionalLight = new DirectionalLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	m_pScene->AddDirectionalLight(pDirectionalLight);

	m_pScene->AddPointLight(new PointLight(glm::vec3(5.0f, 2.0f, -10.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	m_pScene->AddPointLight(new PointLight(glm::vec3(2.0f, 2.0f, -10.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	m_pScene->AddPointLight(new PointLight(glm::vec3(-5.0f, 2.0f, -10.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));

	//m_pScene->AddSpotLight(new SpotLight(glm::vec3(1.0f, 3.0f, 0.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.5f)), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.5f, 0.5f, 1.0f)));

	m_pTextViewFPS = new TextView(0, 720, 200, 50, "FPS");
	m_pTextViewUPS = new TextView(0, 690, 200, 50, "UPS");

	GetGUIManager().Add(m_pTextViewFPS);
	GetGUIManager().Add(m_pTextViewUPS);

	//Audio
	//m_pSoundEffect = new SoundEffect("Resources/Audio/Stereo/Seagulls.wav");
	m_pSoundEffect = new SoundEffect("Resources/Audio/Mono/fart.wav");
	m_pMusic = new Music("Resources/Audio/Music/WavesAndSeagulls.ogg");
	m_pTestAudioSource = new AudioSource(*m_pMusic);
	m_pTestAudioSource->SetPitch(1.0f);
	m_pTestAudioSource->SetLooping(true);
	m_pTestAudioSource->Play();

	AudioListener::SetPosition(glm::vec3(0.0f));

	/*const uint32 level0SizeX = 10;
	const uint32 level0SizeZ = 10;
	uint32 level0[level0SizeX * level0SizeZ] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
	};

	const uint32 level1SizeX = 10;
	const uint32 level1SizeZ = 10;
	uint32 level1[level1SizeX * level1SizeZ] =
	{
		69, 1, 69, 3, 69, 5, 69, 7, 69, 9,
		69, 1, 69, 3, 69, 5, 69, 7, 69, 9,
		69, 1, 69, 3, 69, 5, 69, 7, 69, 9,
		69, 1, 69, 3, 69, 5, 69, 7, 69, 9,
		69, 1, 69, 3, 69, 5, 69, 7, 69, 9,
		69, 1, 69, 3, 69, 5, 69, 7, 69, 9,
		69, 1, 69, 3, 69, 5, 69, 7, 69, 9,
		69, 1, 69, 3, 69, 5, 69, 7, 69, 9,
		69, 1, 69, 3, 69, 5, 69, 7, 69, 9,
		69, 1, 69, 3, 69, 5, 69, 7, 69, 9,
	};

	const uint32 level2SizeX = 5;
	const uint32 level2SizeZ = 8;
	uint32 level2[level2SizeX * level2SizeZ] =
	{
		1337, 420, 69, 5, 1337, 420, 69, 5,
		1337, 420, 69, 5, 1337, 420, 69, 5,
		1337, 420, 69, 5, 1337, 420, 69, 5,
		1337, 420, 69, 5, 1337, 420, 69, 5,
		1337, 420, 69, 5, 1337, 420, 69, 5,
	};

	WorldLevel* worldLevel0 = new WorldLevel(level0, level0SizeX, level0SizeZ);
	WorldLevel* worldLevel1 = new WorldLevel(level1, level1SizeX, level1SizeZ);
	WorldLevel* worldLevel2 = new WorldLevel(level2, level2SizeX, level2SizeZ);

	WorldLevel* worldLevels[3] =
	{
		worldLevel0,
		worldLevel1,
		worldLevel2,
	};

	WorldObject worldObjects[5] =
	{
		{ glm::uvec3(0, 1, 2), 1337, 69 },
		{ glm::uvec3(3, 4, 5), 1337, 420 },
		{ glm::uvec3(6, 7, 8), 1337, 5 },
		{ glm::uvec3(9, 10, 11), 1337, 15 },
		{ glm::uvec3(12, 13, 14), 1337, 8 },
	};

	World* world = new World(worldLevels, 3, worldObjects, 5);
	WorldSerializer::Write("test.json", *world);
	
	Delete(world);*/
	//Create instancing test scene
	m_pInstancingTestScene = new Scene();
	m_pInstancingTestScene->SetSkyBox(new SkyBox(m_pSkyBoxTex));
	
	//Add lights
	m_pInstancingTestScene->AddDirectionalLight(new DirectionalLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.5f, 0.0f)));
	
	//Add camera
	m_pInstancingTestScene->SetCamera(new Camera());
	m_pInstancingTestScene->GetCamera().CreatePerspective(
		glm::radians<float>(90.0f),
		static_cast<float>(GetWindow().GetWidth()) / static_cast<float>(GetWindow().GetHeight()),
		0.01f,
		100.0f);

	m_pInstancingTestScene->GetCamera().SetPos(glm::vec3(0.0f, 0.0f, -3.0f));
	m_pInstancingTestScene->GetCamera().UpdateFromLookAt();

	//Add gameobjects
	GameObject* pGameObject = nullptr;
	constexpr uint32 numObject = 50;

	for (uint32 y = 0; y < numObject; y++)
	{
		for (uint32 x = 0; x < numObject; x++)
		{
			pGameObject = new GameObject();
			pGameObject->SetMesh(MESH::SPHERE);

			if (x % 3 == 0)
			{
				pGameObject->SetMaterial(MATERIAL::RED);
			}
			else if (x % 3 == 1)
			{
				pGameObject->SetMaterial(MATERIAL::GREEN);
			}
			else if (x % 3 == 2)
			{
				pGameObject->SetMaterial(MATERIAL::BLUE);
			}

			pGameObject->SetPosition(glm::vec3(0.0f + (x * 1.0f), 0.0f, 0.0f + (y * 1.0f)));
			pGameObject->SetScale(glm::vec3(0.25f));
			pGameObject->UpdateTransform();

			m_pInstancingTestScene->AddGameObject(pGameObject);
			
			GameObject* pDecalObject = new GameObject();
			pDecalObject->SetDecal(DECAL::BLOOD);
			pDecalObject->SetPosition(pGameObject->GetPosition());
			pDecalObject->UpdateTransform();

			m_pInstancingTestScene->AddGameObject(pDecalObject);
		}
	}

	return;
}

Game::~Game()
{
	DeleteSafe(m_pRenderer);
	DeleteSafe(m_pDebugRenderer);

	DeleteSafe(m_pSkyBoxTex);

	DeleteSafe(m_pScene);
	DeleteSafe(m_pInstancingTestScene);
	
	DeleteSafe(m_pTextViewFPS);
	DeleteSafe(m_pTextViewUPS);
	
	DeleteSafe(m_pSoundEffect);
	DeleteSafe(m_pMusic);
	
	DeleteSafe(m_pTestAudioSource);
	//DeleteSafe(g_Grid);
	DeleteSafe(m_pWorld);

	ResourceHandler::ReleaseResources();
}

void Game::OnResourcesLoaded()
{
	GameObject* pGameObject = nullptr;
	pGameObject = new GameObject();
	pGameObject->SetDecal(DECAL::BLOOD);
	pGameObject->SetPosition(glm::vec3(-6.0f, 2.0f, 0.0f));
	pGameObject->SetScale(glm::vec3(3.0f, 4.0f, 3.0f));
	pGameObject->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	pGameObject->UpdateTransform();
	g_pDecalObject = pGameObject;
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetName("ship");
	pGameObject->SetMaterial(MATERIAL::BOAT);
	pGameObject->SetMesh(MESH::SHIP);
	pGameObject->SetPosition(glm::vec3(5.5f, -3.0f, 12.5f));
	pGameObject->SetScale(glm::vec3(1.0f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	//const GameObject* pBoat = m_pScene->GetGameObject("ship");


	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::GROUND);
	pGameObject->SetMesh(MESH::CLIFF_3_LOW);
	pGameObject->SetPosition(glm::vec3(0.0f, -1.4f, 0.0f));
	pGameObject->SetScale(glm::vec3(0.4f));
	pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::half_pi<float>()));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::RED);
	pGameObject->SetMesh(MESH::CUBE_OBJ);
	pGameObject->SetPosition(glm::vec3(5.0f, 2.0f, -10.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::GREEN);
	pGameObject->SetMesh(MESH::CUBE_OBJ);
	pGameObject->SetPosition(glm::vec3(2.0f, 2.0f, -10.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::BLUE);
	pGameObject->SetMesh(MESH::CUBE_OBJ);
	pGameObject->SetPosition(glm::vec3(-5.0f, 2.0f, -10.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	//Water?? YAAAS
	pGameObject = new GameObject();
	pGameObject->SetIsReflectable(true);
	pGameObject->SetMesh(MESH::QUAD);
	pGameObject->SetScale(glm::vec3(200.0f));
	pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::half_pi<float>()));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	m_pWorld = WorldSerializer::Read("world.json");

	//Enable clipplane for wallmaterial
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->EnableClipPlane(true);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetCullMode(CULL_MODE_NONE);

	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->EnableClipPlane(true);
	ResourceHandler::GetMaterial(MATERIAL::CREW_STANDARD)->EnableClipPlane(true);
	SetClipPlanes();

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

	float x, y, z;
	for (int i = 0; i < 5; i++)
	{
		y = (std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2;
		x = std::rand() % (m_pWorld->GetLevel(y)->GetSizeX() - 2) + 1;
		z = std::rand() % (m_pWorld->GetLevel(y)->GetSizeZ() - 2) + 1;
		g_Crew.AddMember(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec3(x, 0.9f + y, z));
		
		y = (std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2;
		x = std::rand() % (m_pWorld->GetLevel(y)->GetSizeX() - 2) + 1;
		z = std::rand() % (m_pWorld->GetLevel(y)->GetSizeZ() - 2) + 1;
		g_Crew.AddMember(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec3(x, 0.9f + y, z));
		
		y = (std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2;
		x = std::rand() % (m_pWorld->GetLevel(y)->GetSizeX() - 2) + 1;
		z = std::rand() % (m_pWorld->GetLevel(y)->GetSizeZ() - 2) + 1;
		g_Crew.AddMember(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(x, 0.9f + y, z));
	}

	for (int i = 0; i < g_Crew.GetCount(); i++)
	{
		m_pScene->AddGameObject(g_Crew.GetMember(i));
		m_pScene->AddPointLight(g_Crew.GetMember(i)->GetLight());
		g_Crew.GetMember(i)->SetPath(m_pWorld);
		g_Crew.GetMember(i)->UpdateTransform();
	}
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
		case KEY_P:
		{
			m_pTestAudioSource->TogglePause();
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
	m_pScene->OnUpdate(dtS);

	if (cartesianCamera)
	{
		//Cartesian
		static float cartesianCameraSpeed = 5.0f;
		static float cartesianCameraAngularSpeed = 1.5f;

		if (Input::IsKeyDown(KEY_W))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Forward, cartesianCameraSpeed * dtS);
			m_pInstancingTestScene->GetCamera().MoveCartesian(CameraDirCartesian::Forward, cartesianCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_S))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Backwards, cartesianCameraSpeed * dtS);
			m_pInstancingTestScene->GetCamera().MoveCartesian(CameraDirCartesian::Backwards, cartesianCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_A))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Left, cartesianCameraSpeed * dtS);
			m_pInstancingTestScene->GetCamera().MoveCartesian(CameraDirCartesian::Left, cartesianCameraSpeed * dtS);

		}
		else if (Input::IsKeyDown(KEY_D))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Right, cartesianCameraSpeed * dtS);
			m_pInstancingTestScene->GetCamera().MoveCartesian(CameraDirCartesian::Right, cartesianCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_E))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Up, cartesianCameraSpeed * dtS);
			m_pInstancingTestScene->GetCamera().MoveCartesian(CameraDirCartesian::Up, cartesianCameraSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_Q))
		{
			m_pScene->GetCamera().MoveCartesian(CameraDirCartesian::Down, cartesianCameraSpeed * dtS);
			m_pInstancingTestScene->GetCamera().MoveCartesian(CameraDirCartesian::Down, cartesianCameraSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_UP))
		{
			m_pScene->GetCamera().OffsetPitch(cartesianCameraAngularSpeed * dtS);
			m_pInstancingTestScene->GetCamera().OffsetPitch(cartesianCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_DOWN))
		{
			m_pScene->GetCamera().OffsetPitch(-cartesianCameraAngularSpeed * dtS);
			m_pInstancingTestScene->GetCamera().OffsetPitch(-cartesianCameraAngularSpeed * dtS);
		}

		if (Input::IsKeyDown(KEY_LEFT))
		{
			m_pScene->GetCamera().OffsetYaw(-cartesianCameraAngularSpeed * dtS);
			m_pInstancingTestScene->GetCamera().OffsetYaw(-cartesianCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_RIGHT))
		{
			m_pScene->GetCamera().OffsetYaw(cartesianCameraAngularSpeed * dtS);
			m_pInstancingTestScene->GetCamera().OffsetYaw(cartesianCameraAngularSpeed * dtS);
		}

		m_pScene->GetCamera().UpdateFromPitchYaw();
		m_pInstancingTestScene->GetCamera().UpdateFromPitchYaw();
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

	int level;
	for (int i = 0; i < g_Crew.GetCount(); i++) {
		if (Input::IsKeyDown(KEY_ENTER) && !g_Crew.GetMember(i)->IsMoving())
		{
			level = (std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2;
			glm::ivec3 goalPos(std::rand() % (m_pWorld->GetLevel(level)->GetSizeX() - 1), level, std::rand() % (m_pWorld->GetLevel(level)->GetSizeZ() - 1));
			//goalPos = glm::ivec3(18, 1, 1);
			std::cout << i << ": (" << goalPos.x << ", " << goalPos.y << ", " << goalPos.z << ")\n";
			g_Crew.GetMember(i)->FindPath(goalPos);
		}
	}

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
	m_pRenderer->DrawScene(*m_pScene, dtS);
	//m_pRenderer->DrawScene(*m_pInstancingTestScene, dtS);
	
#if defined(DRAW_DEBUG_BOXES)
	m_pDebugRenderer->DrawScene(*m_pScene);
	//m_pDebugRenderer->DrawScene(*m_pInstancingTestScene);
#endif
}

void Game::SetClipPlanes()
{
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 1.8f + (m_CurrentElevation * 2.0f));
	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 2.0f + (m_CurrentElevation * 2.0f));
	ResourceHandler::GetMaterial(MATERIAL::CREW_STANDARD)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 2.0f + (m_CurrentElevation * 2.0f));
}
