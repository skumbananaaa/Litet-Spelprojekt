#include "..\Include\Game.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <World/Grid.h>
#include "..\Include\Crew.h"
#include "..\Include\Path.h"

#if defined(_DEBUG)
#define DRAW_DEBUG_BOXES
#endif

GameObject* g_pDecalObject = nullptr;
Crew g_Crew;
Grid * g_Grid;

float g_Rot = 1.0;

Game::Game() noexcept : 
	Application(false),
	m_pFontRenderer(nullptr),
	m_pRenderer(nullptr),
	m_pDebugRenderer(nullptr),
	m_pScene(nullptr),
	cartesianCamera(true)
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
		m_pSkyBoxTex = new TextureCube(paths, TEX_FORMAT_RGBA, cubeParams);
		m_pScene->SetSkyBox(new SkyBox(m_pSkyBoxTex));
	}

	g_Crew.addMember(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, -2.0f));
	m_pScene->AddGameObject(g_Crew.getMember(0));
	m_pScene->AddPointLight(g_Crew.getMember(0)->GetLight());

	g_Grid = new Grid(MATERIAL::WHITE, glm::ivec2(20, 20), glm::vec3(-10.0f, 0.0f, -10.0f));

	int temp_map[20][20]{
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{1, 3, 3, 3, 2, 2, 2, 3, 3, 3, 5, 5, 2, 2, 2, 2, 2, 4, 4, 1 },
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
			g_Grid->GetTile(glm::ivec2(i, j))->SetID(temp_map[i][j]);
			//g_Grid->SetColor(glm::ivec2(i, j), glm::vec4(temp_map[i][j] / 10.0f, temp_map[i][j] / 10.0f, temp_map[i][j] / 10.0f, 1.0f));
			m_pScene->AddGameObject(g_Grid->GetTile(glm::ivec2(i, j)));
		}
	}

	Camera* pCamera = new Camera(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	float aspect = static_cast<float>(GetWindow().GetWidth()) / static_cast<float>(GetWindow().GetHeight());
	pCamera->CreatePerspective(glm::radians<float>(90.0f), aspect, 0.01f, 100.0f);
	pCamera->UpdateFromPitchYaw();
	m_pScene->SetCamera(pCamera);

	//Lights
	DirectionalLight* pDirectionalLight = new DirectionalLight(glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::vec3(0.0f, 0.5f, 0.5f));
	m_pScene->AddDirectionalLight(pDirectionalLight);

	m_pScene->AddPointLight(new PointLight(glm::vec3(5.0f, 2.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	m_pScene->AddPointLight(new PointLight(glm::vec3(2.0f, 2.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	m_pScene->AddPointLight(new PointLight(glm::vec3(-5.0f, 2.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));

	m_pScene->AddSpotLight(new SpotLight(glm::vec3(1.0f, 3.0f, 0.0f), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(20.5f)), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.5f, 0.5f, 1.0f)));

	m_pTextViewFPS = new TextView(0, 720, 200, 50, "FPS");
	m_pTextViewUPS = new TextView(0, 690, 200, 50, "UPS");

	GetGUIManager().Add(m_pTextViewFPS);
	GetGUIManager().Add(m_pTextViewUPS);

	//Audio
	//m_pSoundEffect = new SoundEffect("Resources/Audio/Stereo/Seagulls.wav");
	m_pSoundEffect = new SoundEffect("Resources/Audio/Mono/fart.wav");
	m_pMusic = new Music("Resources/Audio/Music/CyaronsGate.ogg");
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
}

Game::~Game()
{
	DeleteSafe(m_pFontRenderer);
	DeleteSafe(m_pRenderer);
	DeleteSafe(m_pDebugRenderer);

	DeleteSafe(m_pScene);
	
	Delete(m_pSkyBoxTex);
	DeleteSafe(m_pTextViewFPS);
	DeleteSafe(m_pTextViewUPS);
	
	DeleteSafe(m_pSoundEffect);
	DeleteSafe(m_pMusic);
	
	DeleteSafe(m_pTestAudioSource);

	DeleteSafe(g_Grid);
	Delete(m_pWorld);

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
	pGameObject->SetMaterial(MATERIAL::BOAT);
	pGameObject->SetMesh(MESH::SHIP);
	pGameObject->SetPosition(glm::vec3(0.0f, -0.8f, 0.0f));
	pGameObject->SetScale(glm::vec3(6.0f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

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
	pGameObject->SetPosition(glm::vec3(5.0f, 2.0f, 0.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::GREEN);
	pGameObject->SetMesh(MESH::CUBE_OBJ);
	pGameObject->SetPosition(glm::vec3(2.0f, 2.0f, 0.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	pGameObject = new GameObject();
	pGameObject->SetMaterial(MATERIAL::BLUE);
	pGameObject->SetMesh(MESH::CUBE_OBJ);
	pGameObject->SetPosition(glm::vec3(-5.0f, 2.0f, 0.0f));
	pGameObject->SetScale(glm::vec3(0.25f));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);

	//Water ??
	pGameObject = new GameObject();
	pGameObject->SetMesh(MESH::CUBE);
	pGameObject->SetScale(glm::vec3(15.0f));
	pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::half_pi<float>()));
	pGameObject->UpdateTransform();
	m_pScene->AddGameObject(pGameObject);



	m_pWorld = WorldSerializer::Read("world.json");

	/*for (int level = 0; level < m_pWorld->GetNumLevels(); level++) {
		const uint32* const* map = m_pWorld->GetLevel(level)->GetLevel();
		glm::ivec2 size(m_pWorld->GetLevel(level)->GetSizeX(), m_pWorld->GetLevel(level)->GetSizeZ());
		/*g_Grid = new Grid(glm::ivec2(m_pWorld->GetLevel(level)->GetSizeX(), m_pWorld->GetLevel(level)->GetSizeZ()), glm::vec3(0.0f, 10.0f + 2.0f * level, 0.0f));

		for (int i = 0; i < g_Grid->GetSize().x; i++)
		{
			for (int j = 0; j < g_Grid->GetSize().y; j++)
			{
				g_Grid->GetTile(glm::ivec2(i, j))->SetID(map[i][j]);
				g_Grid->SetColor(glm::ivec2(i, j), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				m_pScene->AddGameObject(g_Grid->GetTile(glm::ivec2(i, j)));
			}
		}*/

		/*Crewmember * CurrentCrewMember = g_Crew.getMember(level);
		CurrentCrewMember->SetPosition(glm::vec3(1.0f, 10.9f + 2.0f * level, 1.0f));
		CurrentCrewMember->SetPath(map, size);

		m_pWorld->GenerateWalls(level);
		glm::vec4 wall;

		for (int i = 0; i < m_pWorld->GetLevel(level)->GetNrOfWalls(); i++) {
			wall = m_pWorld->GetLevel(level)->GetWall(i);
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::BLUE);
			pGameObject->SetMesh(MESH::CUBE);
			pGameObject->SetPosition(glm::vec3(wall.x, 11.0f + 2.0f * level, wall.y));
			pGameObject->SetScale(glm::vec3(wall.z + 0.1f, 2.0f, wall.w + 0.1f));
			pGameObject->UpdateTransform();
			m_pScene->AddGameObject(pGameObject);
		}
	}*/
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

	/*Crewmember * CurrentCrewMember[3] = {
		g_Crew.getMember(0),
		g_Crew.getMember(1),
		g_Crew.getMember(2)
	};
	for (int i = 0; i < 3; i++) {
		if (Input::IsKeyDown(KEY_ENTER) && !CurrentCrewMember[i]->IsMoving())
		{
			glm::ivec2 goalPos(std::rand() % (m_pWorld->GetLevel(i)->GetSizeX() - 1), std::rand() % (m_pWorld->GetLevel(i)->GetSizeZ() - 1));
			std::cout << "(" << goalPos.x << ", " << goalPos.y << ")\n";
			CurrentCrewMember[i]->FindPath(goalPos);
		}
		CurrentCrewMember[i]->FollowPath(dtS);
		CurrentCrewMember[i]->UpdateTransform();
	}*/
}

void Game::OnRender(float dtS)
{
	m_pRenderer->DrawScene(*m_pScene, dtS);
	
#if defined(DRAW_DEBUG_BOXES)
	m_pDebugRenderer->DrawScene(*m_pScene);
#endif
}