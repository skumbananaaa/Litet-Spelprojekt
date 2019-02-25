#include "..\Include\Game.h"
#include <World/Grid.h>
#include <System/Random.h>
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <World/Grid.h>
#include <World/LightManager.h>
#include <Graphics/Renderers/ForwardRenderer.h>
#include <Graphics/Particles/ParticleEmitter.h>

#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/Button.h>
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/Slider.h>

#if defined(_DEBUG)
//#define DRAW_DEBUG_BOXES
#endif

float g_Rot = 1.0;

Game::Game() noexcept 
	: Application(false, 1600, 900, "", true),
	m_pRenderer(nullptr),
	m_pDebugRenderer(nullptr),
	m_pSkyBoxTex(nullptr),
	m_pWorld(nullptr),
	m_pTestAudioSource(nullptr),
	cartesianCamera(false),
	m_CurrentElevation(2)
{
	Logger::SetListener(this);

	m_pTextViewFile = new TextView((GetWindow().GetWidth() - 300) / 2, (GetWindow().GetHeight() - 50) / 2 + 50, 300, 50, "Loading...");
	m_pLoadingBar = new ProgressBar((GetWindow().GetWidth() - 300) / 2, (GetWindow().GetHeight() - 50) / 2, 300, 50);

	GetGUIManager().Add(m_pTextViewFile);
	GetGUIManager().Add(m_pLoadingBar);
}

Game::~Game()
{
	Logger::Save();

	DeleteSafe(m_pRenderer);
	DeleteSafe(m_pDebugRenderer);

	DeleteSafe(m_pSkyBoxTex);

	for (size_t i = 0; i < m_Scenes.size(); i++)
	{
		DeleteSafe(m_Scenes[i]);
	}

	DeleteSafe(m_pWorld);
	DeleteSafe(m_pTextViewScene);
	DeleteSafe(m_pTextViewFile);
	DeleteSafe(m_pLoadingBar);
	DeleteSafe(m_pUICrewMember);
	DeleteSafe(m_pUICrew);
	DeleteSafe(m_PanelLog);

	DeleteSafe(m_pTestAudioSource);

	ScenarioManager::Release();
	LightManager::Release();
}

void Game::OnLogged(const std::string& text) noexcept
{
	glm::vec4 color = m_ListScrollableLog->GetNrOfChildren() % 2 == 0 ? glm::vec4(0.2F, 0.2F, 0.2F, 1.0F) : glm::vec4(0.3F, 0.3F, 0.3F, 1.0F);
	TextView* textView = new TextView(0, 0, m_ListScrollableLog->GetClientWidth(), 40, text);
	textView->SetBackgroundColor(color);
	m_ListScrollableLog->Add(textView);
}

void Game::OnResourceLoading(const std::string& file, float percentage)
{
	m_pTextViewFile->SetText("Loading: " + file);
	m_pLoadingBar->SetPercentage(percentage);
}

void Game::OnResourcesLoaded()
{
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::Init();
#endif

	GetGUIManager().Remove(m_pTextViewFile);
	GetGUIManager().Remove(m_pLoadingBar);

	m_pUICrewMember = new UICrewMember(330, 170);

	m_PanelLog = new Panel(GetWindow().GetWidth() - 350, GetWindow().GetHeight() - 450, 350, 450);
	m_pTextViewLog = new TextView(0, m_PanelLog->GetHeight() - 50, m_PanelLog->GetWidth(), 50, "Loggbok", true);
	m_ListScrollableLog = new ListScrollable(0, 0, m_PanelLog->GetWidth(), m_PanelLog->GetHeight() - m_pTextViewLog->GetHeight());
	m_ListScrollableLog->SetBackgroundColor(glm::vec4(0.15F, 0.15F, 0.15F, 1.0F));
	m_PanelLog->SetDeleteAllChildrenOnDestruction(true);
	m_PanelLog->Add(m_pTextViewLog);
	m_PanelLog->Add(m_ListScrollableLog);

	m_pUIScenario = new UIScenario((GetWindow().GetWidth() - 600) / 2, (GetWindow().GetHeight() - 450) / 2, 600, 450);

	GetGUIManager().Add(m_pUICrewMember);
	GetGUIManager().Add(m_PanelLog);
	GetGUIManager().Add(m_pUIScenario);

	//Set game TextViews
	{
		m_pTextViewScene = new TextView(0, 0, 100, 50, "Scene " + std::to_string(m_SceneId));

		GetGUIManager().Add(m_pTextViewScene);
	}

	//Create Scene
	m_Scenes.push_back(new Scene());

	LightManager::Init(m_Scenes[0], 3);


	//Create renderers
#if defined(DEFERRED_RENDER_PATH)
	m_pRenderer = new DefferedRenderer();
#elif defined(FORWARD_RENDER_PATH)
	m_pRenderer = new ForwardRenderer();
#else
#error "No renderpath defined. Check 'Defines.h'."
#endif

#if defined(_DEBUG)
	m_pDebugRenderer = new DebugRenderer();
#endif
	//m_pRenderer = new OrthographicRenderer();

	//Audio
	{
		AudioListener::SetPosition(glm::vec3(0.0f));
		m_pTestAudioSource = AudioSource::CreateMusicSource(MUSIC::WAVES_AND_SEAGULLS);
		m_pTestAudioSource->SetPitch(1.0f);
		m_pTestAudioSource->SetLooping(true);
		//m_pTestAudioSource->Play();
	}

	//Camera
	Camera* pCamera = new Camera(glm::vec3(-2.0f, 10.0f, 20.0f), glm::vec3(9.0f, 4.0f, 20.0f));
	float aspect = static_cast<float>(GetWindow().GetWidth()) / static_cast<float>(GetWindow().GetHeight());
	pCamera->CreatePerspective(glm::radians<float>(90.0f), aspect, 0.1f, 1000.0f);
	pCamera->UpdateFromPitchYaw();
	m_Scenes[0]->SetCamera(pCamera);

	//Skybox
	{
		m_pSkyBoxTex = new TextureCube(ResourceHandler::GetTexture2D(TEXTURE::HDR));
		m_Scenes[0]->SetSkyBox(new SkyBox(m_pSkyBoxTex));
	}

	//Create GameObjects
	GameObject* pGameObject = nullptr;
	{
		//Blood decal
		{
			pGameObject = new GameObject();
			pGameObject->SetDecal(DECAL::BLOOD);
			pGameObject->SetPosition(glm::vec3(-6.0f, 2.0f, 0.0f));
			pGameObject->SetScale(glm::vec3(3.0f, 4.0f, 3.0f));
			pGameObject->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
			pGameObject->UpdateTransform();
			m_Scenes[0]->AddGameObject(pGameObject);
		}

		//Ground
		{
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::GROUND);
			pGameObject->SetMesh(MESH::CLIFF_3_LOW);
			pGameObject->SetPosition(glm::vec3(0.0f, -1.4f, 0.0f));
			pGameObject->SetScale(glm::vec3(0.4f));
			pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::half_pi<float>()));
			pGameObject->UpdateTransform();
			m_Scenes[0]->AddGameObject(pGameObject);
		}

		//Bottom floor
		{
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::RED);
			pGameObject->SetMesh(MESH::CUBE_OBJ);
			pGameObject->SetPosition(glm::vec3(5.5f, 0.0f, 20.5f));
			pGameObject->SetScale(glm::vec3(10.0f, 0.1f, 40.0f));
			pGameObject->UpdateTransform();
			m_Scenes[0]->AddGameObject(pGameObject);
		}

		//Middle floor
		{
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::GREEN);
			pGameObject->SetMesh(MESH::CUBE_OBJ);
			pGameObject->SetPosition(glm::vec3(5.5f, 2.0f, 20.5f));
			pGameObject->SetScale(glm::vec3(10.0f, 0.1f, 40.0f));
			pGameObject->UpdateTransform();
			m_Scenes[0]->AddGameObject(pGameObject);
		}

		//Top floor
		{
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::BLUE);
			pGameObject->SetMesh(MESH::CUBE_OBJ);
			pGameObject->SetPosition(glm::vec3(5.5f, 4.0f, 20.5f));
			pGameObject->SetScale(glm::vec3(10.0f, 0.1f, 40.0f));
			pGameObject->UpdateTransform();
			m_Scenes[0]->AddGameObject(pGameObject);
		}

		//Ship
		{
			/*pGameObject = new GameObject();
			pGameObject->SetName("ship");
			pGameObject->SetMaterial(MATERIAL::BOAT);
			pGameObject->SetMesh(MESH::SHIP);
			pGameObject->SetPosition(glm::vec3(5.5f, -3.0f, 12.5f));
			pGameObject->SetScale(glm::vec3(1.0f));
			pGameObject->UpdateTransform();
			m_Scenes[0]->AddGameObject(pGameObject);*/
		}
	}

	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetStencilTest(true, FUNC_ALWAYS, 0xff, 1, 0xff);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetFrontFaceStencilOp(STENCIL_OP_KEEP, STENCIL_OP_REPLACE, STENCIL_OP_ZERO);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetBackFaceStencilOp(STENCIL_OP_KEEP, STENCIL_OP_KEEP, STENCIL_OP_REPLACE);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetCullMode(CULL_MODE_NONE);
	
	//Create world
	m_pWorld = WorldSerializer::Read("world.json");

	//Create scenarios
	ScenarioManager::RegisterScenario(new ScenarioFire(m_pWorld));
	ScenarioManager::RegisterScenario(new ScenarioWater(false));

	//Place objects in scene
	int gameObjects = m_pWorld->GetNumWorldObjects();
	for (int i = 0; i < gameObjects; i++)
	{
		WorldObject worldObject = m_pWorld->GetWorldObject(i);
		int32 width = m_pWorld->GetLevel(worldObject.TileId.y)->GetSizeX();
		int32 height = m_pWorld->GetLevel(worldObject.TileId.y)->GetSizeZ();
		int floorLevel = worldObject.TileId.y / 2;
		GameObject* pGameObject = ResourceHandler::CreateGameObject(worldObject.GameObject);
		glm::uvec3 pos = worldObject.TileId;
		pos.x += 1;
		pos.z += 1;
		pGameObject->SetPosition(pos);
		pGameObject->SetRotation(glm::vec4(0, 1, 0, worldObject.Rotation));
		pGameObject->SetRoom(m_pWorld->GetLevel(pos.y)->GetLevel()[pos.x][pos.z]);
		pGameObject->UpdateTransform();
		m_Scenes[0]->AddGameObject(pGameObject);
		m_pWorld->GetLevel(pos.y)->GetLevelData()[pos.x][pos.z].GameObjects.push_back(pGameObject);
	}

	//Generate Door GameObjects
	for (uint32 i = 0; i < m_pWorld->GetNumDoors(); i++)
	{
		glm::vec3 door1 = m_pWorld->GetDoor(i);
		WorldLevel* level = m_pWorld->GetLevel(door1.y);
		float halfWidth = level->GetSizeX() / 2;
		float halfHeight = level->GetSizeZ() / 2;

		for (uint32 j = i + 1; j < m_pWorld->GetNumDoors(); j++)
		{
			glm::vec3 door2 = m_pWorld->GetDoor(j);
			glm::vec3 delta = door1 - door2;
			if (glm::length(delta) <= 1.0)
			{
				glm::vec3 position = (door1 + door2) / 2.0F;

				GameObject* pGameObject = new GameObject();
				pGameObject->SetMaterial(MATERIAL::WHITE);
				pGameObject->SetMesh(MESH::DOOR_FRAME);
				pGameObject->SetPosition(position);
				pGameObject->SetRotation(glm::vec4(0, 1, 0, delta.z * glm::half_pi<float>()));
				pGameObject->UpdateTransform();
				m_Scenes[0]->AddGameObject(pGameObject);

				pGameObject = new GameObjectDoor();
				pGameObject->SetPosition(position);
				pGameObject->SetRotation(glm::vec4(0, 1, 0, delta.z * glm::half_pi<float>()));
				pGameObject->UpdateTransform();
				m_Scenes[0]->AddGameObject(pGameObject);

				level->GetLevelData()[(int32)door1.x][(int32)door1.z].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR] = pGameObject;
				level->GetLevelData()[(int32)door2.x][(int32)door2.z].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR] = pGameObject;

				break;
			}
		}
	}

	//Generate Ladder GameObjects
	for (uint32 i = 0; i < m_pWorld->GetNumStairs(); i++)
	{
		glm::ivec3 stair = m_pWorld->GetStairs()[i];
		WorldLevel* level = m_pWorld->GetLevel(stair.y);
		float halfWidth = level->GetSizeX() / 2;
		float halfHeight = level->GetSizeZ() / 2;

		glm::vec3 position = ((glm::vec3)stair);

		const uint32* const* grid = level->GetLevel();
		uint32 myId = grid[stair.x][stair.z];
		float rotation = 0;

		if (grid[stair.x + 1][stair.z] != myId)
		{
			rotation = glm::half_pi<float>() * 2.0F;
		}
		else if (grid[stair.x - 1][stair.z] != myId)
		{
			rotation = 0.0F;
		}
		else if (grid[stair.x][stair.z + 1] != myId)
		{
			rotation = glm::half_pi<float>();
		}
		else if (grid[stair.x][stair.z - 1] != myId)
		{
			rotation = glm::half_pi<float>() * 3.0F;
		}

		GameObject* pGameObject = new GameObject();
		pGameObject->SetMaterial(MATERIAL::WHITE);
		pGameObject->SetMesh(MESH::LADDER);
		pGameObject->SetPosition(position);
		pGameObject->SetRotation(glm::vec4(0, 1, 0, rotation));
		pGameObject->UpdateTransform();
		m_Scenes[0]->AddGameObject(pGameObject);
	}

	//BOB
	{
		pGameObject = new GameObject();
		pGameObject->SetMaterial(MATERIAL::ANIMATED_MODEL);
		pGameObject->SetAnimatedMesh(MESH::ANIMATED_MODEL);
		pGameObject->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
		//pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(90.0f)));
		pGameObject->SetScale(glm::vec3(1.0f));
		pGameObject->UpdateTransform();
		m_Scenes[0]->AddGameObject(pGameObject);
	}

	//Water?? YAAAS
	{
		pGameObject = new GameObject();
		pGameObject->SetIsReflectable(true);
		pGameObject->SetMesh(MESH::QUAD);
		pGameObject->SetMaterial(MATERIAL::WATER_OUTDOOR);
		pGameObject->SetScale(glm::vec3(200.0f));
		pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::half_pi<float>()));
		pGameObject->UpdateTransform();
		m_Scenes[0]->AddGameObject(pGameObject);
	}

	//Reflector for water
	PlanarReflector* pReflector = new PlanarReflector(glm::vec3(0.0f, 1.0f, 0.0f), 0.01f);
	m_Scenes[0]->AddPlanarReflector(pReflector);
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetStencilTest(true, FUNC_NOT_EQUAL, 0x00, 1, 0xff);
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetStencilOp(STENCIL_OP_KEEP, STENCIL_OP_KEEP, STENCIL_OP_KEEP);
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetPlanarReflector(pReflector);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetCullMode(CULL_MODE_NONE);
	////Enable clipplane for wallmaterial
	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetCullMode(CULL_MODE_NONE);

	//SetClipPlanes(0);

	// Generate rooms
	m_pWorld->GenerateRooms();
	for (int level = 0; level < m_pWorld->GetNumLevels(); level += 2)
	{
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
			
			m_Scenes[0]->AddGameObject(pGameObject);
		}
	}

	//Generate water
	m_pWorld->GenerateWater(m_Scenes[0]);

	//Lights
	{
		DirectionalLight* pDirectionalLight = new DirectionalLight(glm::vec4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		m_Scenes[0]->AddDirectionalLight(pDirectionalLight);

		for (uint32 i = 0; i < MAX_ROOMS_VISIBLE; i++)
		{
			m_Scenes[0]->AddRoomLight(new PointLight(m_pWorld->GetRoom(0)->GetCenter(), glm::vec4(2.0f, 2.0f, 2.0f, 2.0f)));
			m_RoomLightsTimers.push_back(0.0f);
			m_ActiveRooms.push_back(0);
		}

		//m_Scenes[0]->AddSpotLight(new SpotLight(glm::vec3(6.0f, 5.9f, 10.0f), glm::cos(glm::radians(45.5f)), glm::cos(glm::radians(60.5f)), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
		//m_Scenes[0]->AddSpotLight(new SpotLight(glm::vec3(6.0f, 5.9f, 25.0f), glm::cos(glm::radians(45.5f)), glm::cos(glm::radians(60.5f)), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	}

	//Crew
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

	m_Crew.AddMember(DEFAULT_LIGHT, glm::vec3(10.0f, 0.9f + 4.0f, 10.0f), 100, names[0]);
	//m_Scenes[0]->AddSpotLight(m_Crew.GetMember(i)->GetTorch());
	//m_Scenes[0]->AddPointLight(m_Crew.GetMember(i)->GetLight());
	m_Crew.GetMember(0)->SetPath(m_pWorld);
	m_Crew.GetMember(0)->SetRoom(m_pWorld->GetLevel((int)4.0f)->GetLevel()[(int)10.0f][(int)10.0f]);
	m_Crew.GetMember(0)->SetHidden(true);
	m_Crew.GetMember(0)->UpdateTransform();
	m_Scenes[0]->AddGameObject(m_Crew.GetMember(0));

	float x, y, z;
	for (int i = 1; i < NUM_CREW; i++)
	{
		y = (std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2;
		x = std::rand() % (m_pWorld->GetLevel(y)->GetSizeX() - 2) + 1;
		z = std::rand() % (m_pWorld->GetLevel(y)->GetSizeZ() - 2) + 1;
		m_Crew.AddMember(DEFAULT_LIGHT, glm::vec3(x, 0.9f + y, z), 100, names[i % 15]);
		//m_Scenes[0]->AddSpotLight(m_Crew.GetMember(i)->GetTorch());
		//m_Scenes[0]->AddPointLight(m_Crew.GetMember(i)->GetLight());
		m_Crew.GetMember(i)->SetPath(m_pWorld);
		m_Crew.GetMember(i)->SetRoom(m_pWorld->GetLevel((int)y)->GetLevel()[(int)x][(int)z]);
		m_Crew.GetMember(i)->SetHidden(true);
		m_Crew.GetMember(i)->UpdateTransform();
		m_Scenes[0]->AddGameObject(m_Crew.GetMember(i));
	}

	std::vector<Crewmember*> members;
	for (int i = 0; i < m_Crew.GetCount(); i++)
	{
		members.push_back(m_Crew.GetMember(i));
	}

	m_pUICrew = new UICrew(0, GetWindow().GetHeight() - 150, 200, 500, members);

	m_pRenderer->SetWorldBuffer(*m_Scenes[m_SceneId], m_pWorld);

	/*_______________________________________________________________________________________________________________*/
	//SCENE2

	m_Scenes.push_back(new Scene());

	//Camera
	pCamera = new Camera(glm::vec3(-2.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	pCamera->CreatePerspective(glm::radians<float>(90.0f), GetWindow().GetAspectRatio(), 0.1f, 1000.0f);
	pCamera->UpdateFromPitchYaw();
	m_Scenes[1]->SetCamera(pCamera);

	//Skybox
	m_Scenes[1]->SetSkyBox(new SkyBox(m_pSkyBoxTex));

	//Lights
	m_Scenes[1]->AddDirectionalLight(new DirectionalLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	
	//Create GameObjects
	pGameObject = nullptr;
	for (uint32 i = 0; i < 5; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(MESH::SPHERE);
		pGameObject->SetMaterial(MATERIAL::RED);
		pGameObject->SetPosition(glm::vec3(0.0f, 0.0f, i * 2.0f));
		pGameObject->UpdateTransform();
		m_Scenes[1]->AddGameObject(pGameObject);
	}

	for (uint32 i = 0; i < 5; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(MESH::SPHERE);
		pGameObject->SetMaterial(MATERIAL::RED_1);
		pGameObject->SetPosition(glm::vec3(2.0f, 0.0f, i * 2.0f));
		pGameObject->UpdateTransform();
		m_Scenes[1]->AddGameObject(pGameObject);
	}

	for (uint32 i = 0; i < 5; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(MESH::SPHERE);
		pGameObject->SetMaterial(MATERIAL::RED_2);
		pGameObject->SetPosition(glm::vec3(4.0f, 0.0f, i * 2.0f));
		pGameObject->UpdateTransform();
		m_Scenes[1]->AddGameObject(pGameObject);
	}

	for (uint32 i = 0; i < 5; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(MESH::SPHERE);
		pGameObject->SetMaterial(MATERIAL::RED_3);
		pGameObject->SetPosition(glm::vec3(6.0f, 0.0f, i * 2.0f));
		pGameObject->UpdateTransform();
		m_Scenes[1]->AddGameObject(pGameObject);
	}

	for (uint32 i = 0; i < 5; i++)
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(MESH::SPHERE);
		pGameObject->SetMaterial(MATERIAL::RED_4);
		pGameObject->SetPosition(glm::vec3(8.0f, 0.0f, i * 2.0f));
		pGameObject->UpdateTransform();
		m_Scenes[1]->AddGameObject(pGameObject);
	}

	//Water
	{
		pGameObject = new GameObject();
		pGameObject->SetMesh(MESH::QUAD);
		pGameObject->SetIsReflectable(true);
		pGameObject->SetMaterial(MATERIAL::WATER_OUTDOOR);
		pGameObject->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, -glm::radians<float>(90.0f)));
		pGameObject->SetScale(glm::vec3(30.0f));
		pGameObject->UpdateTransform();
		m_Scenes[1]->AddGameObject(pGameObject);

		PlanarReflector* pReflector = new PlanarReflector(glm::vec3(0.0f, 1.0f, 0.0f), 0.01f);
		m_Scenes[1]->AddPlanarReflector(pReflector);
	}

	ParticleEmitter* pEmitter = new ParticleEmitter();
	pEmitter->SetParticleBlendMode(PARTICLE_NORMAL);
	pEmitter->SetTexture(TEXTURE::SMOKE);
	pEmitter->SetTimeToLive(7.0f);
	pEmitter->SetConeAngle(glm::radians<float>(40.0f));
	pEmitter->SetSpeed(0.1f, 0.4f);
	pEmitter->SetScale(glm::vec2(0.2f), glm::vec2(1.0f));
	pEmitter->SetBeginColor(glm::vec4(0.2f, 0.2f, 0.2f, 0.3f));
	pEmitter->SetEndColor(glm::vec4(0.05f, 0.05f, 0.05f, 0.3f));
	pEmitter->SetPosition(glm::ivec3(-4.0f, 0.0f, -4.0f));
	pEmitter->SetParticlesPerSeconds(5);
	pEmitter->UpdateTransform();
	m_Scenes[1]->AddGameObject(pEmitter);

	pEmitter = new ParticleEmitter();
	pEmitter->SetPosition(glm::ivec3(-4.0f, 0.0f, -6.0f));
	pEmitter->SetParticleBlendMode(PARTICLE_ADDITIVE);
	pEmitter->SetTexture(TEXTURE::SMOKE);
	pEmitter->SetTimeToLive(1.2f);
	pEmitter->SetScale(glm::vec2(0.2f), glm::vec2(1.0f));
	pEmitter->SetConeAngle(glm::radians<float>(30.0f));
	pEmitter->SetSpeed(0.7f, 2.0f);
	pEmitter->SetBeginColor(glm::vec4(1.0f, 1.0f, 0.3f, 1.0f));
	pEmitter->AddColorNode(glm::vec4(1.0f, 0.92f, 0.03f, 1.0f), 0.3f);
	pEmitter->SetEndColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pEmitter->SetParticlesPerSeconds(5);
	pEmitter->UpdateTransform();
	m_Scenes[1]->AddGameObject(pEmitter);

	//((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetPlanarReflector(pReflector);

	m_SceneId = 0;
}

void Game::OnUpdateLoading(float dtS)
{
	
}

void Game::OnRenderLoading(float dtS)
{
}

void Game::OnKeyUp(KEY keycode)
{
	switch (keycode)
	{
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
			m_Scenes[m_SceneId]->ExtendScene();
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
		case KEY_TAB:
		{
			m_SceneId = (m_SceneId + 1) % m_Scenes.size();
			m_pTextViewScene->SetText("Scene " + std::to_string(m_SceneId));
			((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetPlanarReflector(m_Scenes[m_SceneId]->GetPlanarReflectors()[0]);
			m_pRenderer->SetWorldBuffer(*m_Scenes[m_SceneId], m_pWorld);
			break;
		}
		case KEY_R:
		{
			ShowCrewmember(0);
			ScenarioManager::OnVisibilityChange(m_pWorld, m_Scenes[m_SceneId], m_ActiveRooms);
			break;
		}
	}

	Application::OnKeyDown(keycode);
}

void Game::OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	if (Input::IsKeyDown(KEY_LEFT_ALT))
	{
		if (!cartesianCamera)
		{
			if (Input::IsButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
			{
				const float cameraRotationSensitivity = 0.005f;
				glm::vec2 deltaPosition = cameraRotationSensitivity * (position - lastPosition);

				m_Scenes[m_SceneId]->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateX, deltaPosition.x);
				m_Scenes[m_SceneId]->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateY, -deltaPosition.y);

				m_pUICrewMember->SetCrewMember(nullptr);
			}

			if (Input::IsButtonDown(MouseButton::MOUSE_BUTTON_RIGHT))
			{
				const float cameraMoveSensitivityX = 0.5f;
				const float cameraMoveSensitivityY = 0.025f;
				glm::vec2 deltaPosition = cameraMoveSensitivityY * (position - lastPosition);
				glm::vec3 forward(0.0f);
				forward.x = m_Scenes[m_SceneId]->GetCamera().GetFront().x;
				forward.z = m_Scenes[m_SceneId]->GetCamera().GetFront().z;
				m_Scenes[m_SceneId]->GetCamera().MoveWorldCoords(-forward * deltaPosition.y, true);
				m_Scenes[m_SceneId]->GetCamera().MoveLocalCoords(glm::vec3(cameraMoveSensitivityX * deltaPosition.x, 0.0f, 0.0f), true);

				m_pUICrewMember->SetCrewMember(nullptr);
			}
		}
	}
	else
	{
		PickCrew(true);
	}
}

void Game::OnMouseReleased(MouseButton mousebutton, const glm::vec2& position)
{
	bool clickedOnGUI = false;
	for (GUIObject* pObject : GetGUIManager().GetChildren())
	{
		if (pObject->OwnsPoint(position))
		{
			clickedOnGUI = true;
			break;
		}
	}

	if (!clickedOnGUI)
	{
		switch (mousebutton)
		{
			case MOUSE_BUTTON_LEFT:
			{
				if (!Input::IsKeyDown(KEY_LEFT_ALT) && m_pWorld != nullptr)
				{
					PickPosition();
				}
				break;
			}
			case MOUSE_BUTTON_RIGHT:
			{
				if (!Input::IsKeyDown(KEY_LEFT_ALT) && m_pWorld != nullptr)
				{
					PickCrew(false);
				}
				break;
			}
		}
	}
}

void Game::OnMouseScroll(const glm::vec2& offset, const glm::vec2& position)
{
	bool clickedOnGUI = false;
	for (GUIObject* pObject : GetGUIManager().GetChildren())
	{
		if (pObject->OwnsPoint(position))
		{
			clickedOnGUI = true;
			break;
		}
	}

	if (!clickedOnGUI)
	{
		if (!cartesianCamera)
		{
			if (Input::IsKeyDown(KEY_LEFT_ALT))
			{
				if (offset.y > 0.0f)
				{
					m_Scenes[m_SceneId]->GetCamera().MoveWorldCoords(glm::vec3(0.0f, 1.0f, 0.0f), true);
				}
				else
				{
					m_Scenes[m_SceneId]->GetCamera().MoveWorldCoords(glm::vec3(0.0f, -1.0f, 0.0f), true);
				}

				SetClipPlanes(m_SceneId);
			}
			else
			{
				const float cameraZoomSensitivity = 0.1f;
				m_Scenes[m_SceneId]->GetCamera().MoveRelativeLookAt(PosRelativeLookAt::Zoom, cameraZoomSensitivity * offset.y);
			}
		}
	}
}

void Game::OnUpdate(float dtS)
{
	ScenarioManager::Update(dtS, m_pWorld, m_Scenes[m_SceneId], m_ActiveRooms);

	static float dist = 0.0f;
	dist += 0.02f * dtS;
	((WaterOutdoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR))->SetDistortionFactor(dist);
	((WaterIndoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_INDOOR))->SetDistortionFactor(dist);

	std::vector<PointLight*>& roomLights = m_Scenes[m_SceneId]->GetRoomLights();

	for (uint32 i = 0; i < roomLights.size(); i++)
	{
		m_RoomLightsTimers[i] += dtS;
		if (m_RoomLightsTimers[i] >= 5.0f)
		{
			roomLights[i]->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
			m_RoomLightsTimers[i] = 0.0f;
			m_pWorld->GetRoom(m_ActiveRooms[i])->SetActive(false);
			m_ActiveRooms[i] = 1;
			ScenarioManager::OnVisibilityChange(m_pWorld, m_Scenes[m_SceneId], m_ActiveRooms);
		}
	}

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::StartTimer(CPU_PROFILER_SLOT_0);
#endif
	m_pWorld->Update(m_Scenes[m_SceneId], dtS);
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::EndTimer("World Update took %.3f ms", CPU_PROFILER_SLOT_0);
#endif

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::StartTimer(CPU_PROFILER_SLOT_1);
#endif
	m_Scenes[m_SceneId]->OnUpdate(dtS);
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::EndTimer("Scene Update took %.3f ms", CPU_PROFILER_SLOT_1);
#endif

	float cartesianCameraSpeed = 5.0F;
	float cartesianCameraAngularSpeed = 1.5F;

	if (cartesianCamera)
	{
		glm::vec3 localMove(0.0f);

		if (Input::IsKeyDown(KEY_W))
		{
			localMove.z = cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}
		else if (Input::IsKeyDown(KEY_S))
		{
			localMove.z = -cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}

		if (Input::IsKeyDown(KEY_A))
		{
			localMove.x = cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}
		else if (Input::IsKeyDown(KEY_D))
		{
			localMove.x = -cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}

		if (Input::IsKeyDown(KEY_E))
		{
			localMove.y = cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}
		else if (Input::IsKeyDown(KEY_Q))
		{
			localMove.y = -cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}

		m_Scenes[m_SceneId]->GetCamera().MoveLocalCoords(localMove);

		if (Input::IsKeyDown(KEY_UP))
		{
			m_pUICrewMember->SetCrewMember(nullptr);
			m_Scenes[m_SceneId]->GetCamera().OffsetPitch(cartesianCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_DOWN))
		{
			m_Scenes[m_SceneId]->GetCamera().OffsetPitch(-cartesianCameraAngularSpeed * dtS);
			m_pUICrewMember->SetCrewMember(nullptr);
		}

		if (Input::IsKeyDown(KEY_LEFT))
		{
			m_Scenes[m_SceneId]->GetCamera().OffsetYaw(-cartesianCameraAngularSpeed * dtS);
			m_pUICrewMember->SetCrewMember(nullptr);
		}
		else if (Input::IsKeyDown(KEY_RIGHT))
		{
			m_Scenes[m_SceneId]->GetCamera().OffsetYaw(cartesianCameraAngularSpeed * dtS);
			m_pUICrewMember->SetCrewMember(nullptr);
		}

		m_Scenes[m_SceneId]->GetCamera().UpdateFromPitchYaw();
	}
	else
	{
		m_Scenes[m_SceneId]->GetCamera().UpdateFromLookAt();
	}

	GameObject* pCameraLookAt = m_Scenes[m_SceneId]->GetGameObject("cameraLookAt");
	if (pCameraLookAt)
	{
		pCameraLookAt->SetPosition(m_Scenes[m_SceneId]->GetCamera().GetLookAt());
	}

	AudioListener::SetPosition(m_Scenes[m_SceneId]->GetCamera().GetPosition());
	AudioListener::SetOrientation(m_Scenes[m_SceneId]->GetCamera().GetFront(), m_Scenes[m_SceneId]->GetCamera().GetUp());

	if (Input::IsKeyPressed(KEY_NUMPAD_2))
	{
		if (m_CurrentElevation > 0)
		{
			m_CurrentElevation--;
		}
		
		SetClipPlanes(m_SceneId);
		std::cout << "Elevation: " << m_CurrentElevation << std::endl;
	}

	if (Input::IsKeyPressed(KEY_NUMPAD_8))
	{
		if (m_CurrentElevation < 2)
		{
			m_CurrentElevation++;
		}

		SetClipPlanes(m_SceneId); 
		std::cout << "Elevation: " << m_CurrentElevation << std::endl;
	}

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::Update(dtS);
	CPUProfiler::PrintTime();
#endif
}

void Game::OnRender(float dtS)
{
	m_pRenderer->DrawScene(*m_Scenes[m_SceneId], m_pWorld, dtS);

#if defined(DRAW_DEBUG_BOXES)
	m_pDebugRenderer->DrawScene(*m_Scenes[m_SceneId]);
#endif
	
}

void Game::PickPosition() {
	glm::vec3 rayDir = GetRay(Input::GetMousePosition(), GetWindow().GetWidth(), GetWindow().GetHeight());
	glm::vec3 rayOrigin = m_Scenes[m_SceneId]->GetCamera().GetPosition();
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

			float extension = m_Scenes[m_SceneId]->GetExtension();
			if (pointOnSurface.x > extension * d / 2.0f)
			{
				pointOnSurface.x -= extension * floor(pointOnSurface.y / 2.0f);
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

void Game::PickCrew(bool hover)
{
	Crewmember* crewmember = RayTestCrewmembers();

	if (crewmember)
	{
		if (hover)
		{
			if (!crewmember->IsHovered())
			{
				crewmember->OnHovered();
			}
		}
		else
		{
			crewmember->OnPicked();
		}
	}
	else if (hover)
	{
		for (int i = 0; i < m_Crew.GetCount(); i++)
		{
			if (m_Crew.GetMember(i)->IsHovered())
			{
				m_Crew.GetMember(i)->OnNotHovered();
			}
		}
	}
}

glm::vec3 Game::GetRay(const glm::vec2 & mousepos, uint32 windowWidth, uint32 windowHeight)
{
	glm::vec4 rayDir4((2.0f * mousepos.x) / windowWidth - 1.0f, 1.0f - (2.0f * mousepos.y) / windowHeight, -1.0, 1.0);
	rayDir4 = m_Scenes[m_SceneId]->GetCamera().GetInverseProjectionMatrix() * rayDir4;
	rayDir4 = glm::vec4(glm::vec2(rayDir4), -1.0, 0.0);
	rayDir4 = m_Scenes[m_SceneId]->GetCamera().GetInverseViewMatrix() * rayDir4;

	glm::vec3 rayDir = glm::normalize(glm::vec3(rayDir4));

	return rayDir;
}

void Game::ShowCrewmember(uint32 crewmember)
{
	glm::ivec3 tile = m_Crew.GetMember(crewmember)->GetTile();
	uint32 roomIndex = m_pWorld->GetLevel(tile.y * 2)->GetLevel()[tile.x][tile.z];

	if (!m_pWorld->GetRoom(roomIndex)->IsActive())
	{
		const glm::vec3& roomCenter = m_pWorld->GetRoom(roomIndex)->GetCenter();
		std::vector<PointLight*>& roomLights = m_Scenes[m_SceneId]->GetRoomLights();

		roomLights[m_CurrentLight]->SetPosition(roomCenter);
		m_RoomLightsTimers[m_CurrentLight] = 0.0f;
		m_ActiveRooms[m_CurrentLight] = roomIndex;
		m_pWorld->GetRoom(roomIndex)->SetActive(true);
		m_CurrentLight = (m_CurrentLight + 1) % roomLights.size();
	}
}

Crewmember* Game::RayTestCrewmembers()
{
	glm::vec3 rayDir = GetRay(Input::GetMousePosition(), GetWindow().GetWidth(), GetWindow().GetHeight());
	glm::vec3 rayOrigin = m_Scenes[m_SceneId]->GetCamera().GetPosition();

	float lastT = -1;
	uint32 id = -1;

	for (int i = 0; i < m_Crew.GetCount(); i++)
	{
		int32 t = m_Crew.GetMember(i)->TestAgainstRay(rayDir, rayOrigin, m_Scenes[m_SceneId]->GetExtension());

		if (t > 0 && lastT == -1 || t >= 0 && t < lastT)
		{
			lastT = t;
			id = i;
		}
	}

	if (id != -1)
	{
		return m_Crew.GetMember(id);
	}
	return nullptr;
}


void Game::SetClipPlanes(uint32 scene)
{
	/*ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 1.8f + (m_CurrentElevation * 2.0f));
	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 2.0f + (m_CurrentElevation * 2.0f));
	ResourceHandler::GetMaterial(MATERIAL::CREW_STANDARD)->SetClipPlane(glm::vec3(0.0f, -1.0f, 0.0f), 2.0f + (m_CurrentElevation * 2.0f));*/
	
	/*float elevation = glm::clamp((glm::floor(m_Scenes[scene]->GetCamera().GetLookAt().y / 2.0f)), 0.0f, 2.0f);
	((WallMaterial*)ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD))->SetClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 1.99f + (elevation * 2.0f)), 1);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetLevelClipPlane(glm::vec4(0.0f, -1.0f, 0.0f, 1.99f + (elevation * 2.0f)));
	*/
	
	//m_pRenderer->SetClipDistance(glm::vec4(0.0f, -1.0f, 0.0f, 1.99f + (elevation * 2.0f)), 1);
	//m_pRenderer->SetClipDistance(glm::vec4(0.0f, -1.0f, 0.0f, 1.99f + (elevation * 2.0f)), 1);
}

Crewmember* Game::GetCrewmember(uint32 shipNumber)
{
	return m_Crew.GetMember(shipNumber);
}

UICrewMember* Game::GetUICrewMember() noexcept
{
	return m_pUICrewMember;
}

UIScenario* Game::GetUIScenario() noexcept
{
	return m_pUIScenario;
}

Scene* Game::GetScene()
{
	return m_Scenes[m_SceneId];
}

Game* Game::GetGame()
{
	return (Game*)(&Application::GetInstance());
}
