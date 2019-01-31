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
	m_pDebugRenderer = new DebugRenderer();

	m_pScene = new Scene();
	m_pTestMesh = IndexedMesh::CreateIndexedMeshFromFile("Resources/Meshes/ship.obj");
	m_pGroundTestMesh = IndexedMesh::CreateIndexedMeshFromFile("Resources/Meshes/cliff_3_low.obj");
	m_pSphereMesh = IndexedMesh::CreateIndexedMeshFromFile("Resources/Meshes/cube.obj");

	{
		TextureParams params = {};
		params.Wrap = TEX_PARAM_REPEAT;
		params.MinFilter = TEX_PARAM_LINEAR;
		params.MagFilter = TEX_PARAM_LINEAR;

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
	m_pScene->AddGameObject(pGameObject);

	g_Crew.addMember(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, -2.0f));
	g_Crew.addMember(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, -2.0f));
	g_Crew.addMember(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, -2.0f));
	for (int i = 0; i < g_Crew.getCount(); i++) {
		m_pScene->AddGameObject(g_Crew.getMember(i));
		m_pScene->AddPointLight(g_Crew.getMember(i)->GetLight());
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
	
	m_pWorld = WorldSerializer::Read("world.json");

	m_pWallMesh = IndexedMesh::CreateCube();

	for (int level = 0; level < m_pWorld->GetNumLevels(); level++) {
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

		Crewmember * CurrentCrewMember = g_Crew.getMember(level);
		CurrentCrewMember->SetPosition(glm::vec3(1.0f, 10.9f + 2.0f * level, 1.0f));
		CurrentCrewMember->SetPath(map, size);

		m_pWorld->GenerateWalls(level);
		glm::vec4 wall;

		for (int i = 0; i < m_pWorld->GetLevel(level)->GetNrOfWalls(); i++) {
			wall = m_pWorld->GetLevel(level)->GetWall(i);
			pGameObject = new GameObject();
			pGameObject->SetMaterial(m_pBlueMaterial);
			pGameObject->SetMesh(m_pWallMesh);
			pGameObject->SetPosition(glm::vec3(wall.x, 11.0f + 2.0f * level, wall.y));
			pGameObject->SetScale(glm::vec3(wall.z + 0.1f, 2.0f, wall.w + 0.1f));
			pGameObject->UpdateTransform();
			m_pScene->AddGameObject(pGameObject);
		}
	}
}

Game::~Game()
{
	DeleteSafe(m_pBloodTexture);
	DeleteSafe(m_pBloodNormal);

	DeleteSafe(m_pFontRenderer);
	DeleteSafe(m_pRenderer);
	DeleteSafe(m_pDebugRenderer);

	DeleteSafe(m_pScene);
	
	DeleteSafe(m_pSphereMesh);
	DeleteSafe(m_pTestMesh);
	DeleteSafe(m_pWaterMesh);
	DeleteSafe(m_pGroundTestMesh);
	
	Delete(m_pWallMesh);
	DeleteSafe(m_pBoatTexture);
	DeleteSafe(m_pBoatNormalMap);
	
	DeleteSafe(m_pDecal);
	DeleteSafe(m_pRedMaterial);
	DeleteSafe(m_pGreenMaterial);
	DeleteSafe(m_pBlueMaterial);
	DeleteSafe(m_pBoatMaterial);
	DeleteSafe(m_pGroundMaterial);
	
	DeleteSafe(m_pTextViewFPS);
	DeleteSafe(m_pTextViewUPS);
	
	DeleteSafe(m_pSoundEffect);
	DeleteSafe(m_pMusic);
	
	DeleteSafe(m_pTestAudioSource);

	DeleteSafe(g_Grid);
	Delete(m_pWorld);

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

	Crewmember * CurrentCrewMember[3] = {
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
	}
}

void Game::OnRender(float dtS)
{
	m_pRenderer->DrawScene(*m_pScene, dtS);
	
#if defined(DRAW_DEBUG_BOXES)
	m_pDebugRenderer->DrawScene(*m_pScene);
#endif
}